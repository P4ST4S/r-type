/*
** EPITECH PROJECT, 2023
** CrossPlatformNetwork
** File description:
** ANetwork
*/

#include "ANetwork.hpp"

cpn::ANetwork::ANetwork()
{
}

cpn::ANetwork::~ANetwork()
{
    this->stopReception();
    this->removeAllConnection();
}






// CONNECTION MANAGEMENT

bool cpn::ANetwork::removeConnection(cpn::connectionID_t cid) noexcept
{
    bool mutexLocked = false;
    try {
        cpn::connectionData_t &connectionData = this->getConnection(cid);

        this->_connectionMut.lock();
        mutexLocked = true;
        cpn::Utils::closeSocket(connectionData.fd);
        this->_connectionFds.erase(connectionData.fd);
        this->_connections.erase(cid._id);
        this->_connectionMut.unlock();

        LOG(this->_loggersName, "Connection: " << cid << " removed.");
        return (true);
    } catch (const std::exception &e) {
        if (mutexLocked)
            this->_connectionMut.unlock();
    }
    LOG(this->_loggersName, "Failed to remove: " << cid << ", connection doesn't exist.");
    return (false);
}

bool cpn::ANetwork::removeConnection(const std::string &ip, uint16_t port) noexcept
{
    return (this->removeConnection(cpn::Utils::getConnectionID(ip, port)));
}

void cpn::ANetwork::removeAllConnection(void) noexcept
{
    while (!this->_connections.empty())
        this->removeConnection(this->_connections.begin()->first);
}

void cpn::ANetwork::removeAllDisconnectedConnection(void) noexcept
{
    std::unordered_map<uint64_t, connectionData_t>::iterator it = this->_connections.begin();

    while (it != this->_connections.end()) {
        if (!it->second.connected) {
            this->removeConnection(it->first);
            it = this->_connections.begin();
        } else {
            ++it;
        }
    }
}


bool cpn::ANetwork::addConnection(const struct sockaddr_in &sockAddr, int fd) noexcept
{
    cpn::connectionID_t cid = cpn::Utils::getConnectionID(sockAddr);

    if (this->connectionExist(cid))
        return (false);
    this->_connectionMut.lock();
    memcpy(&(this->_connections[cid._id]), &cid, sizeof(cpn::connectionID_t));
    this->_connections[cid._id].connected = true;
    this->_connections[cid._id].fd = fd;
    this->_connections[cid._id].lastActivity = std::time(nullptr);
    this->_connectionFds[fd].first.fd = fd;
    #ifdef _WIN32
        this->_connectionFds[fd].first.events = POLLIN;
    #else
        this->_connectionFds[fd].first.events = POLLIN | POLLPRI | POLLHUP | POLLERR;
    #endif
    this->_connectionFds[fd].second = cid;
    this->_connectionMut.unlock();
    LOG(this->_loggersName, "Connection: " << cid << " added.");
    return (true);
}

bool cpn::ANetwork::clearConnectionBuffer(int protocol, cpn::connectionID_t cid) noexcept
{
    try {
        cpn::connectionData_t &connectionData = this->getConnection(cid);
        this->_connectionMut.lock();
        if (protocol & CPN_UDP)
            connectionData.udpBuf.clear();
        if (protocol & CPN_TCP)
            connectionData.tcpBuf.clear();
        this->_connectionMut.unlock();
        return (true);
    } catch (const std::exception &e) {
        return (false);
    }
}

std::string cpn::ANetwork::getIP(int protocol) const
{
    if (!this->_dataIsSet)
        throw std::runtime_error("Server is not started.");
    if (protocol & CPN_TCP)
        return (cpn::Utils::getIP(this->_tcpSockaddr));
    return (cpn::Utils::getIP(this->_udpSockaddr));
}

uint32_t cpn::ANetwork::getIPInt(int protocol) const
{
    if (!this->_dataIsSet)
        throw std::runtime_error("Server is not started.");
    if (protocol & CPN_TCP)
        return (cpn::Utils::getIPInt(this->_tcpSockaddr));
    return (cpn::Utils::getIPInt(this->_udpSockaddr));
}

int cpn::ANetwork::getSocket(int protocol) const
{
    if (!this->_dataIsSet)
        throw std::runtime_error("Server is not started.");
    if (protocol & CPN_TCP)
        return (this->_tcpSock);
    return (this->_udpSock);
}

const sockaddr_in &cpn::ANetwork::getSockaddr(int protocol) const
{
    if (!this->_dataIsSet)
        throw std::runtime_error("Server is not binded.");
    if (protocol & CPN_TCP)
        return (this->_tcpSockaddr);
    return (this->_udpSockaddr);
}

uint16_t cpn::ANetwork::getPort() const
{
    if (!this->_dataIsSet)
        throw std::runtime_error("No port initialized.");
    return (this->_port);
}

cpn::connectionID_t cpn::ANetwork::getConnectionId() const
{
    if (!this->_dataIsSet)
        throw std::runtime_error("No port initialized.");
    return (cpn::Utils::getConnectionID(this->_udpSockaddr));
}

bool cpn::ANetwork::isReceptionThreadRunning(void) const noexcept
{
    return (this->_recvRunning);
}

ssize_t cpn::ANetwork::getMaxConnection(void) const noexcept
{
    return (this->_maxConnection);
}

size_t cpn::ANetwork::getConnectionCount(void) const noexcept
{
    return (this->_connections.size());
}

bool cpn::ANetwork::connectionSlotAvailable(void) const noexcept
{
    if (this->_maxConnection < 0)
        return (true);
    return (this->_connections.size() < this->_maxConnection);
}

std::deque<cpn::connectionID_t> cpn::ANetwork::getConnectionList(void) noexcept
{
    std::deque<cpn::connectionID_t> list;
    this->_connectionMut.lock();
    std::unordered_map<uint64_t, cpn::connectionData_t>::iterator it = this->_connections.begin();
    std::unordered_map<uint64_t, cpn::connectionData_t>::iterator itEnd = this->_connections.end();

    while (it != itEnd) {
        list.push_back(it->second.cid);
        it++;
    }
    this->_connectionMut.unlock();
    return (std::move(list));
}

cpn::connectionData_t &cpn::ANetwork::getConnection(cpn::connectionID_t cid)
{
    this->_connectionMut.lock();
    if (this->_connections.find(cid._id) == this->_connections.end()) {
        this->_connectionMut.unlock();
        throw std::runtime_error("Connection id not found.");
    }
    this->_connectionMut.unlock();
    return (this->_connections[cid._id]);
}

const cpn::connectionData_t &cpn::ANetwork::getConnection(cpn::connectionID_t cid) const
{
    // std::cout << "Looking for cid: " << cid << std::endl;
    // for (auto it = this->_connections.begin(); it != this->_connections.end(); it++) {
    //     std::cout << "cid: " << static_cast<cpn::connectionID_t>(it->first) << " " << it->second.cid << std::endl;
    // }
    if (this->_connections.find(cid._id) == this->_connections.end()) {
        throw std::runtime_error("Connection id not found.");
    }
    return (this->_connections.at(cid._id));
}

bool cpn::ANetwork::connectionExist(cpn::connectionID_t cid)
{
    this->_connectionMut.lock();
    bool exist = this->_connections.find(cid._id) != this->_connections.end();
    this->_connectionMut.unlock();
    return (exist);
}

ssize_t cpn::ANetwork::getConnectionBufferSize(int protocol, cpn::connectionID_t cid) noexcept
{
    try {
        cpn::connectionData_t &connectionData = this->getConnection(cid);
        if (protocol & CPN_UDP)
            return (connectionData.udpBuf.size());
        else if (protocol & CPN_TCP)
            return (connectionData.tcpBuf.size());
    } catch (const std::exception &e) {
        return (-1);
    }
    return (-1);
}

std::vector<struct pollfd> &cpn::ANetwork::generatePollFds(void) noexcept
{
    std::unordered_map<int, std::pair<struct pollfd, cpn::connectionID_t>>::iterator it;
    std::vector<struct pollfd>::iterator pollIt;

    for (pollIt = this->_pollFds.begin(); pollIt != this->_pollFds.end(); pollIt++)
        this->_connectionFds[pollIt->fd].first = *pollIt;
    this->_pollFds.clear();
    for (it = this->_connectionFds.begin(); it != this->_connectionFds.end(); it++) {
        try {
            if (this->getConnection(this->getFdConnectionId(it->first)).connected)
                this->_pollFds.push_back(it->second.first);
        } catch (const std::exception &e) {
            this->_pollFds.push_back(it->second.first);
        }
    }
    return (this->_pollFds);
}

cpn::connectionID_t cpn::ANetwork::getFdConnectionId(int fd)
{
    if (this->_connectionFds.find(fd) == this->_connectionFds.end())
        throw std::runtime_error("Unknown connection fd.");
    return (this->_connectionFds[fd].second);
}

const std::string &cpn::ANetwork::getClassName(void) const
{
    return (this->_loggersName);
}

#ifdef _WIN32
    bool cpn::ANetwork::initWsa(void)
    {
        if (this->_wsaRunning)
            return (true);
        this->_wsaRunning = (WSAStartup(MAKEWORD(2, 2), &(this->_wsa)) == 0);
        if (!this->_wsaRunning)
            std::cerr << "WSAStartup failed." << std::endl;
        return (this->_wsaRunning);
    }

    void cpn::ANetwork::stopWsa(void)
    {
        if (this->_wsaRunning)
            WSACleanup();
        this->_wsaRunning = false;
    }
#endif

bool cpn::ANetwork::stopReception(void) noexcept
{
    if (!this->_recvRunning)
        return (false);
    LOG(this->_loggersName, "Stopping reception thread...");
    this->_recvRunning = false;

    // Utils::setSockAddr(clientAddr, "127.0.0.1", this->_port);
    // cpn::Utils::sendTo(this->_udpSock, clientAddr, nullptr, 0);
    // LOG(this->_loggersName + "/UDP", "Sent 0/0 byte(s) to 127.0.0.1:" << this->_port << ".");

    // cpn::Utils::sendToSocket(this->_tcpSock, nullptr, 0);
    // LOG(this->_loggersName + "/TCP, "Sent 0/0 byte(s) to 127.0.0.1:" << this->_port << ".");

    cpn::Utils::sendToTcp("127.0.0.1", this->_port, nullptr, 0);
    cpn::Utils::sendToUdp("127.0.0.1", this->_port, nullptr, 0);

    this->_udpRecvThread.join();
    this->_tcpRecvThread.join();
    return (true);
}

ssize_t cpn::ANetwork::genericSend(int protocol, cpn::connectionID_t cid, const std::deque<uint8_t> &buffer, size_t size) const
{
    std::string protocols;
    int fd;
    ssize_t sentSize = 0;
    struct sockaddr_in clientAddr;
    std::vector<uint8_t> tmpBuffer(buffer.begin(), buffer.end());

    if (protocol & CPN_UDP) {
        // ++(cid._port);
        cpn::Utils::setSockAddr(clientAddr, cid._ip, cid._port);
        sentSize = cpn::Utils::sendTo(this->_udpSock, clientAddr, tmpBuffer.data(), size);
        if (sentSize >= 0) {
            LOG(this->_loggersName + "/UDP", "Sent " << sentSize << "/" << size << " byte(s) to " << cid << ".");
        } else {
            LOG(this->_loggersName + "/UDP", "Error while sending data to " << cid << ".");
        }
    }
    if (protocol & CPN_TCP) {
        fd = this->getConnection(cid).fd;
        sentSize = cpn::Utils::sendToSocket(fd, tmpBuffer.data(), size);
        if (sentSize >= 0) {
            LOG(this->_loggersName + "/TCP", "Sent " << sentSize << "/" << size << " byte(s) to " << cid << ".");
        } else {
            LOG(this->_loggersName + "/TCP", "Error while sending data to " << cid << ".");
        }
    }
    return (sentSize);

}

std::deque<uint8_t> cpn::ANetwork::genericReceive(int protocol, cpn::connectionID_t cid, size_t size)
{
    std::deque<uint8_t> data;

    try {
        if (protocol & CPN_UDP)
            data = this->getConnection(cid).udpBuf.readIn(size);
        else if (protocol & CPN_TCP)
            data = this->getConnection(cid).tcpBuf.readIn(size);
        // LOG(this->_loggersName, "Readed " << data.size() << "/" << size << " byte(s) from " << cid << " buffer.");
    } catch (const std::exception &e) {
        LOG(this->_loggersName, "Invalid connection id: " << cid << ".");
    }
    return (std::move(data));
}

std::deque<uint8_t> cpn::ANetwork::genericReceiveFromAny(int protocol, cpn::connectionID_t &cid, size_t size)
{
    std::deque<uint8_t> data;
    this->_connectionMut.lock();
    std::unordered_map<uint64_t, cpn::connectionData_t>::iterator it = this->_connections.begin();
    std::unordered_map<uint64_t, cpn::connectionData_t>::iterator itEnd = this->_connections.end();

    cid._id = 0;
    while (it != itEnd) {
        if ((protocol & CPN_UDP) && it->second.udpBuf.size() > 0) {
            data = it->second.udpBuf.readIn(size);
            cid = it->second.cid;
            break;
        } else if ((protocol & CPN_TCP) && it->second.tcpBuf.size() > 0) {
            data = it->second.tcpBuf.readIn(size);
            cid = it->second.cid;
            break;
        }
        it++;
    }
    this->_connectionMut.unlock();
    return (std::move(data));
}
