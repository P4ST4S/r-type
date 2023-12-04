/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** CrossPlatformServer
*/

#include "Server.hpp"






// INITIAL

cpn::Server::Server()
{
    this->setMaxConnection(-1);
    this->_loggersName = SRV_LOG_NAME;
    #ifdef _WIN32
        this->initWsa();
    #endif
}

cpn::Server::~Server()
{
    this->stop();
    #ifdef _WIN32
        this->stopWsa();
    #endif
    LOG(this->_loggersName, "Server class destroyed!");
}

bool cpn::Server::start(uint16_t port) noexcept
{
    // if (port == UINT16_MAX) {
    //     std::cerr << "Cannot use the port: " << UINT16_MAX <<
    //     " because UDP connection would bind port: " << UINT16_MAX + 1 << std::endl;
    //     return (false);
    // }
    LOG(this->_loggersName, "Attempting starting on port " << port);
    this->_dataIsSet = false;
    if (!this->initSockets() ||
    !this->bindPort(port) ||
    listen(this->_tcpSock, 4) < 0) {
        this->stop();
        return (false);
    }

    this->_dataIsSet = true;
    this->_connectionFds[this->_tcpSock].first.fd = this->_tcpSock;
    this->_connectionFds[this->_tcpSock].first.events = POLLIN;
    this->_connectionFds[this->_tcpSock].second = this->getConnectionId();
    this->startReception();

    LOG(this->_loggersName, "Started! Listening on port " << this->_port);

    return (true);
}

bool cpn::Server::stop() noexcept
{
    if (this->_recvRunning)
        this->stopReception();
    if (this->_socketRunning)
        this->closeSockets();
    this->_socketRunning = false;
    this->_binded = false;
    this->_recvRunning = false;
    this->removeAllConnection();

    LOG(this->_loggersName, "Stopped!");
    return true;
}

bool cpn::Server::startReception(void) noexcept
{
    if (this->_recvRunning)
        return (false);
    this->_recvRunning = true;
    this->_udpRecvThread = std::thread(runUdpReception, this);
    this->_tcpRecvThread = std::thread(runTcpReception, this, true);
    return (true);
}






// CONNECTION MANAGEMENT

bool cpn::Server::connectTo(const std::string &ip, uint16_t port) noexcept
{
    sockaddr_in sockAddr;
    int connectSock = cpn::Utils::tcpSocket();

    if (!cpn::Utils::socketCreationSuccess(connectSock))
        return (false);
    cpn::Utils::setSockAddr(sockAddr, ip, port);
    if (connect(connectSock, reinterpret_cast<const struct sockaddr *>(&sockAddr), sizeof(sockAddr)) < 0) {
        cpn::Utils::closeSocket(connectSock);
        return (false);
    }
    // std::cout << cpn::Utils::getConnectionID(sockAddr) << std::endl;
    return (this->addConnection(sockAddr, connectSock));
}





// DATA MANAGEMENT

std::deque<uint8_t> cpn::Server::receive(int protocol, connectionID_t cid, size_t size) noexcept
{
    return (this->genericReceive(protocol, cid, size));
}

std::deque<uint8_t> cpn::Server::receive(int protocol, connectionID_t cid) noexcept
{
    if (protocol & CPN_UDP)
        return (this->receive(CPN_UDP, cid, this->getConnection(cid).udpBuf.size()));
    else if (protocol & CPN_TCP)
        return (this->receive(CPN_TCP, cid, this->getConnection(cid).tcpBuf.size()));
    return (std::deque<uint8_t>());
}

std::deque<uint8_t> cpn::Server::receiveFromAny(int protocol, connectionID_t &cid, size_t size) noexcept
{
    return (this->genericReceiveFromAny(protocol, cid, size));
}

std::deque<uint8_t> cpn::Server::receiveFromAny(int protocol, connectionID_t &cid) noexcept
{
    if (protocol & CPN_UDP)
        return (this->receiveFromAny(CPN_UDP, cid, this->getConnection(cid).udpBuf.size()));
    else if (protocol & CPN_TCP)
        return (this->receiveFromAny(CPN_TCP, cid, this->getConnection(cid).tcpBuf.size()));
    return (std::deque<uint8_t>());
}

ssize_t cpn::Server::send(int protocol, connectionID_t cid, const std::deque<uint8_t> &buffer, size_t size) const noexcept
{
    return (this->genericSend(protocol, cid, buffer, size));
}

ssize_t cpn::Server::send(int protocol, connectionID_t cid, const std::deque<uint8_t> &buffer) const noexcept
{
    return (this->send(protocol, cid, buffer, buffer.size()));
}

std::deque<ssize_t> cpn::Server::multiSend(int protocol, const std::deque<cpn::connectionID_t> &cid, const std::deque<uint8_t> &buffer, size_t size) const noexcept
{
    std::deque<ssize_t> bytesList;
    std::deque<cpn::connectionID_t>::const_iterator it;

    for (it = cid.begin(); it != cid.end(); it++)
        bytesList.push_back(this->send(protocol, *it, buffer, size));
    return (std::move(bytesList));
}

std::deque<ssize_t> cpn::Server::multiSend(int protocol, const std::deque<cpn::connectionID_t> &cid, const std::deque<uint8_t> &buffer) const noexcept
{
    return (this->multiSend(protocol, cid, buffer, buffer.size()));
}

void cpn::Server::writeInConnectionBuffer(int protocol, cpn::connectionID_t cid, const std::deque<uint8_t> &data) noexcept
{
    LOG(this->_loggersName, "Server status: " <<
    ((this->getLock()) ? "Locked" : "Unlocked") << " | " <<
    "Player connected: " << this->_connections.size() << "/" <<
    this->getMaxConnection());


    // if (protocol == CPN_UDP)
    //     --cid._port;
    if (this->connectionExist(cid)) {
        this->_connectionMut.lock();
        if (protocol & CPN_UDP)
            this->_connections[cid._id].udpBuf.writeIn(data);
        if (protocol & CPN_TCP)
            this->_connections[cid._id].tcpBuf.writeIn(data);
        this->_connections[cid._id].lastActivity = std::time(nullptr);
        this->_connectionMut.unlock();
        LOG(this->_loggersName, data.size() << " byte(s) accepted from " << cid);
    } else {
        if (this->_lock)
            this->sendDataWhenLock(protocol, cid);
        else if (!this->connectionSlotAvailable())
            this->sendDataWhenMaxConnection(protocol, cid);
        else
            this->sendDataWhenConnectionNotConnected(protocol, cid);
        LOG(this->_loggersName, data.size() << " byte(s) ignored from " << cid);
    }
}





// SETTERS

void cpn::Server::setLock(bool lock) noexcept
{
    this->_lock = lock;
}

void cpn::Server::setMaxConnection(ssize_t maxConnection) noexcept
{
    this->_maxConnection = maxConnection;
}

void cpn::Server::dataToSendWhenRemove(std::deque<uint8_t> data) noexcept
{
    this->_dataWhenRemove = data;
}

void cpn::Server::dataToSendWhenLock(std::deque<uint8_t> data) noexcept
{
    this->_dataWhenLock = data;
}

void cpn::Server::dataToSendWhenMaxConnection(std::deque<uint8_t> data) noexcept
{
    this->_dataWhenMaxConnection = data;
}

void cpn::Server::dataToSendWhenConnectionNotConnected(std::deque<uint8_t> data) noexcept
{
    this->_dataWhenConnectionNotConnected = data;
}


void cpn::Server::sendDataWhenRemove(int protocol, cpn::connectionID_t cid) noexcept
{
    if (this->_dataWhenRemove.size() > 0)
        this->send(protocol, cid, this->_dataWhenRemove);
}

void cpn::Server::sendDataWhenLock(int protocol, cpn::connectionID_t cid) noexcept
{
    if (this->_dataWhenLock.size() > 0)
        this->send(protocol, cid, this->_dataWhenLock);
}

void cpn::Server::sendDataWhenMaxConnection(int protocol, cpn::connectionID_t cid) noexcept
{
    if (this->_dataWhenMaxConnection.size() > 0)
        this->send(protocol, cid, this->_dataWhenMaxConnection);
}

void cpn::Server::sendDataWhenConnectionNotConnected(int protocol, cpn::connectionID_t cid) noexcept
{
    if (this->_dataWhenConnectionNotConnected.size() > 0)
        this->send(protocol, cid, this->_dataWhenConnectionNotConnected);
}




// GETTERS

bool cpn::Server::getLock(void) const noexcept
{
    return (this->_lock);
}


bool cpn::Server::bindPort(uint16_t port)
{
    int out;
    sockaddr_in localAddress;
    socklen_t localAddressSize = sizeof(localAddress);

    if (this->_binded)
        return (true);
    cpn::Utils::setSockAddr(this->_tcpSockaddr, INADDR_ANY, port);
    this->_port = cpn::Utils::bindSocket(this->_tcpSock, this->_tcpSockaddr);
    if (this->_port < 0) {
        std::cerr << "TCP bind creation failed" << std::endl;
        return (false);
    }
    cpn::Utils::setSockAddr(this->_udpSockaddr, INADDR_ANY, this->_port);
    if (cpn::Utils::bindSocket(this->_udpSock, this->_udpSockaddr) < 0) {
        std::cerr << "UDP bind creation failed" << std::endl;
        return (false);
    }
    this->_binded = true;
    return (this->_binded);
}

bool cpn::Server::initSockets(void)
{
    this->_udpSock = cpn::Utils::udpSocket();
    this->_socketRunning = cpn::Utils::socketCreationSuccess(this->_udpSock);
    if (!this->_socketRunning) {
        std::cerr << "UDP socket creation failed" << std::endl;
        this->_udpSock = -1;
        return (false);
    }
    this->_tcpSock = cpn::Utils::tcpSocket();
    this->_socketRunning = cpn::Utils::socketCreationSuccess(this->_tcpSock);
    if (!this->_socketRunning) {
        std::cerr << "TCP socket creation failed" << std::endl;
        this->_tcpSock = -1;
    }
    return (this->_socketRunning);
}

void cpn::Server::closeSockets(void)
{
    if (this->_udpSock != -1) {
        cpn::Utils::closeSocket(this->_udpSock);
        this->_udpSock = -1;
    }
    if (this->_tcpSock != -1) {
        cpn::Utils::closeSocket(this->_tcpSock);
        this->_tcpSock = -1;
    }
}
