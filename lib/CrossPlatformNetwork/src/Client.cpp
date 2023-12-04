/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** CrossPlatformClient
*/

#include "Client.hpp"


cpn::Client::Client()
{
    this->_loggersName = CLI_LOG_NAME;
    this->_dataIsSet = false;
    #ifdef _WIN32
        this->initWsa();
    #endif
}

cpn::Client::Client(const std::string& ip, uint16_t port) :
cpn::Client::Client()
{
    this->connectTo(ip, port);
}

cpn::Client::~Client()
{
    this->clearConnection();
    #ifdef _WIN32
        this->stopWsa();
    #endif
}

void cpn::Client::writeInConnectionBuffer(int protocol, cpn::connectionID_t cid, const std::deque<uint8_t> &data) noexcept
{
    // if (protocol == CPN_UDP)
    //     --cid._port;
    if (this->connectionExist(cid)) {
        this->_connectionMut.lock();
        if (protocol == CPN_UDP)
            this->_connections[cid._id].udpBuf.writeIn(data);
        if (protocol == CPN_TCP)
            this->_connections[cid._id].tcpBuf.writeIn(data);
        // this->_connections[cid._id].lastActivity = std::time(nullptr);
        this->_connectionMut.unlock();
        LOG(CLI_LOG_NAME, data.size() << " byte(s) accepted from " << cid);
    } else {
        LOG(CLI_LOG_NAME, data.size() << " byte(s) ignored from " << cid);
    }
}

void cpn::Client::clearConnection(void) noexcept
{
    this->stopReception();
    this->removeAllConnection();
    if (this->_udpSock != -1)
        cpn::Utils::closeSocket(this->_udpSock);

}

bool cpn::Client::connectTo(const std::string& ip, uint16_t port) noexcept
{
    // if (port == UINT16_MAX) {
    //     std::cerr << "Cannot use the port: " << UINT16_MAX <<
    //     " because UDP connection would bind port: " << UINT16_MAX + 1 << std::endl;
    //     return (false);
    // }
    this->disconnect();
    if (!this->initTcpConnection(ip, port) ||
    !this->initUdpConnection(ip, this->_port)) {
        this->clearConnection();
        return (false);
    }

    this->_dataIsSet = true;
    this->startReception();
    this->_targetCid = this->getConnectionList()[0];
    LOG(CLI_LOG_NAME, "Connection established with " << this->_targetCid << "! Client on port " << this->_port);
    return (true);
}

bool cpn::Client::isConnected(void) const noexcept
{
    if (!this->_dataIsSet || this->getConnectionCount() == 0)
        return (false);
    try {
        const cpn::connectionData_t &cdata = this->getConnection(this->_targetCid);
        return (cdata.connected);
    } catch (const std::exception &e) {
    }
    return (false);

}

bool cpn::Client::disconnect(void) noexcept
{
    if (!this->isConnected())
        return (false);
    this->_dataIsSet = false;
    this->clearConnection();
    LOG(CLI_LOG_NAME, "Connection ended with " << this->_targetCid);
    return (true);
}

bool cpn::Client::isAddressSet(void) const noexcept
{
    return (this->_dataIsSet);
}

bool cpn::Client::startReception(void) noexcept
{
    if (this->_recvRunning)
        return (false);
    this->_recvRunning = true;
    this->_udpRecvThread = std::thread(runUdpReception, this);
    this->_tcpRecvThread = std::thread(runTcpReception, this, false);
    return (true);
}

ssize_t cpn::Client::send(int protocol, const std::deque<uint8_t> &data, size_t size) const
{
    return (this->genericSend(protocol, this->_targetCid, data, size));
}

ssize_t cpn::Client::send(int protocol, const std::deque<uint8_t> &data) const
{
    return (this->send(protocol, data, data.size()));
}

std::deque<uint8_t> cpn::Client::receive(int protocol, size_t size)
{
    this->throwWhenAddressNotSet();
    return (this->genericReceive(protocol, this->_targetCid, size));
}

std::deque<uint8_t> cpn::Client::receive(int protocol)
{
    this->throwWhenAddressNotSet();
    return (this->receive(protocol, this->getConnectionBufferSize(protocol, this->_targetCid)));
}

std::string cpn::Client::getTargetIP(int protocol) const
{
    this->throwWhenAddressNotSet();
    return (cpn::Utils::getIP(this->_targetCid));
}

uint32_t cpn::Client::getTargetIPInt(int protocol) const
{
    this->throwWhenAddressNotSet();
    return (this->_targetCid._ip);
}

uint16_t cpn::Client::getTargetPort(void) const
{
    this->throwWhenAddressNotSet();
    return (this->_targetCid._port);
}

cpn::connectionID_t cpn::Client::getTargetConnectionID(void) const
{
    this->throwWhenAddressNotSet();
    return (this->_targetCid);
}

void cpn::Client::throwWhenAddressNotSet(void) const
{
    if (!this->_dataIsSet)
        throw std::runtime_error("Address not set");
}

bool cpn::Client::initTcpConnection(const std::string& ip, uint16_t port) noexcept
{
    sockaddr_in localAddr;
    socklen_t localAddrSize = sizeof(localAddr);


    this->_tcpSock = cpn::Utils::tcpSocket();
    if (!cpn::Utils::socketCreationSuccess(this->_tcpSock)) {
        std::cerr << "TCP socket creation failed" << std::endl;
        return (false);
    }

    try {
        cpn::Utils::setSockAddr(this->_tcpSockaddr, ip, port);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return (false);
    }
    if (connect(this->_tcpSock, reinterpret_cast<const struct sockaddr *>(&this->_tcpSockaddr), sizeof(this->_tcpSockaddr)) < 0)
        return (false);
    getsockname(this->_tcpSock, reinterpret_cast<struct sockaddr*>(&localAddr), &localAddrSize);
    this->_port = cpn::Utils::getPort(localAddr);
    // std::cout << ip << " " << port << " " << this->_port << std::endl;
    this->addConnection(this->_tcpSockaddr, this->_tcpSock);
    return (true);
}

bool cpn::Client::initUdpConnection(const std::string& ip, uint16_t port) noexcept
{
    this->_udpSock = cpn::Utils::udpSocket();
    if (!cpn::Utils::socketCreationSuccess(this->_udpSock)) {
        std::cerr << "UDP socket creation failed" << std::endl;
        return (false);
    }

    cpn::Utils::setSockAddr(this->_udpSockaddr, INADDR_ANY, port);
    // std::cout << ip << " " << port << std::endl;
    if (cpn::Utils::bindSocket(this->_udpSock, this->_udpSockaddr) < 0) {
        std::cerr << "UDP bind failed: " << strerror(errno) << " " << errno << std::endl;
        return (false);
    }
    return (true);
}
