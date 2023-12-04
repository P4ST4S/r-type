/*
** EPITECH PROJECT, 2023
** CrossPlatformNetwork
** File description:
** ReceptionFunctions
*/

#include "ReceptionFunctions.hpp"


void runUdpReception(cpn::INetwork *network)
{
    sockaddr_in addrIn = {0};
    std::deque<uint8_t> data;
    cpn::connectionID_t cid;

    while (network->isReceptionThreadRunning()) {
        data = cpn::Utils::recvFrom(network->getSocket(CPN_UDP), MAX_BUF_SIZE, addrIn, false);
        cid = cpn::Utils::getConnectionID(addrIn);
        LOG(network->getClassName() << "/UDP", "Received from: " << cid << ": "
            << data.size() << "/" << MAX_BUF_SIZE << " byte(s)");
        if (network->isReceptionThreadRunning())
            network->writeInConnectionBuffer(CPN_UDP, cid, data);
    }
    LOG(network->getClassName() << "/UDP", "Reception thread stopped!");
}

static void checkForNewConnection(cpn::Server *server, std::vector<struct pollfd> &pollFds)
{
    int newFd;
    int tcpSock = server->getSocket(CPN_TCP);
    sockaddr_in newAddr = {0};
    socklen_t addrSize = sizeof(newAddr);
    std::vector<struct pollfd>::iterator it;
    cpn::connectionID_t cid;

    for (it = pollFds.begin(); it != pollFds.end() && server->isReceptionThreadRunning(); ++it) {
        if (it->fd == tcpSock) {
            if (it->revents & POLLIN) {
                newFd = accept(tcpSock, (struct sockaddr *)&newAddr, &addrSize);
                LOG(server->getClassName() << "/TCP", "New connection detected!");
                server->addConnection(newAddr, newFd);
                cid = server->getFdConnectionId(newFd);
                if (server->getLock()) {
                    LOG(server->getClassName() << "/TCP", "Server locked, connection refused!");
                    server->sendDataWhenLock(CPN_TCP, cid);
                    server->removeConnection(cid);
                } else if (server->getMaxConnection() != -1 && server->getConnectionCount() > server->getMaxConnection()) {
                    LOG(server->getClassName() << "/TCP", "No more connection slot available, connection refused!");
                    server->sendDataWhenMaxConnection(CPN_TCP, cid);
                    server->removeConnection(cid);
                }
            }
            break;
        }
    }
    server->generatePollFds();
}

static void handleConnection(cpn::INetwork *network, std::vector<struct pollfd> &pollFds, bool acceptNewConnection)
{
    int tcpSock = network->getSocket(CPN_TCP);
    ssize_t bytes;
    cpn::connectionID_t cid;
    std::deque<uint8_t> data;
    std::vector<struct pollfd>::iterator it;
    sockaddr_in addr = {0};

    it = pollFds.begin();
    while (it != pollFds.end() && pollFds.size() > 0 && network->isReceptionThreadRunning()) {
        if (!acceptNewConnection || it->fd != tcpSock) {
            cid = network->getFdConnectionId(it->fd);
            cpn::connectionData_t &connectionData = network->getConnection(cid);
            if (connectionData.connected) {
                if (it->revents & (POLLHUP | POLLERR)) {
                    connectionData.connected = false;
                    LOG(network->getClassName() << "/TCP", cid << " disconnected");
                    network->generatePollFds();
                    it = pollFds.begin();
                    continue;
                } else if (it->revents & (POLLIN | POLLPRI)) {
                    cpn::Utils::setSockAddr(addr, cid);
                    data = cpn::Utils::recvFromSocket(it->fd, MAX_BUF_SIZE);
                    if (data.size() == 0) {
                        connectionData.connected = false;
                        LOG(network->getClassName() << "/TCP", cid << " disconnected");
                        network->generatePollFds();
                        it = pollFds.begin();
                        continue;
                    }
                    LOG(network->getClassName() << "/TCP", "Received from: " <<
                    cid << ": " << data.size() << "/" << MAX_BUF_SIZE << " byte(s)");
                    network->writeInConnectionBuffer(CPN_TCP, cid, data);
                }
            }
            it->revents = 0;
        }
        ++it;
    }
}

void runTcpReception(cpn::INetwork *network, bool acceptNewConnection)
{
    int timeout = 100;
    int socketActivity;
    std::vector<struct pollfd> &pollFds = network->generatePollFds();

    while (network->isReceptionThreadRunning()) {
        // for (size_t i = 0; i < pollFds.size(); ++i) {
        //     std::cout << pollFds[i].fd << ": ";
        //     std::cout << ((pollFds[i].events & POLLIN) ? "POLLIN " : "");
        //     std::cout << ((pollFds[i].events & POLLPRI) ? "POLLPRI " : "");
        //     std::cout << ((pollFds[i].events & POLLOUT) ? "POLLOUT " : "");
        //     std::cout << ((pollFds[i].events & POLLERR) ? "POLLERR " : "");
        //     std::cout << ((pollFds[i].events & POLLHUP) ? "POLLHUP " : "");
        //     std::cout << ((pollFds[i].events & POLLNVAL) ? "POLLNVAL " : "");
        //     // std::cout << ((pollFds[i].events & POLLRDNORM) ? "POLLRDNORM " : "");
        //     // std::cout << ((pollFds[i].events & POLLRDBAND) ? "POLLRDBAND " : "");
        //     // std::cout << ((pollFds[i].events & POLLWRNORM) ? "POLLWRNORM " : "");
        //     // std::cout << ((pollFds[i].events & POLLWRBAND) ? "POLLWRBAND " : "");
        //     std::cout << std::endl;
        // }
        if (pollFds.size() == 0) {
            LOG(network->getClassName() << "/TCP", "No more active connection detected!");
            break;
        }
        #ifdef _WIN32
            socketActivity = WSAPoll(pollFds.data(), pollFds.size(), timeout);
        #else
            socketActivity = poll(pollFds.data(), pollFds.size(), timeout);
        #endif
        if (socketActivity < 0) {
            #ifdef _WIN32
                std::cerr << "WSAPoll failed: " << WSAGetLastError() << " stopping TCP reception" << std::endl;
            #else
                std::cerr << "Poll failed stopping TCP reception" << std::endl;
            #endif
            break;
        }
        try {
            if (socketActivity > 0) {
                LOG(network->getClassName() << "/TCP", "Activity detected! " << network->isReceptionThreadRunning());
                // for (size_t i = 0; i < pollFds.size(); ++i) {
                //     std::cout << pollFds[i].fd << ": ";
                //     std::cout << ((pollFds[i].revents & POLLIN) ? "POLLIN " : "");
                //     std::cout << ((pollFds[i].revents & POLLPRI) ? "POLLPRI " : "");
                //     std::cout << ((pollFds[i].revents & POLLOUT) ? "POLLOUT " : "");
                //     std::cout << ((pollFds[i].revents & POLLERR) ? "POLLERR " : "");
                //     std::cout << ((pollFds[i].revents & POLLHUP) ? "POLLHUP " : "");
                //     std::cout << ((pollFds[i].revents & POLLNVAL) ? "POLLNVAL " : "");
                //     // std::cout << ((pollFds[i].revents & POLLRDNORM) ? "POLLRDNORM " : "");
                //     // std::cout << ((pollFds[i].revents & POLLRDBAND) ? "POLLRDBAND " : "");
                //     // std::cout << ((pollFds[i].revents & POLLWRNORM) ? "POLLWRNORM " : "");
                //     // std::cout << ((pollFds[i].revents & POLLWRBAND) ? "POLLWRBAND " : "");
                //     std::cout << std::endl;
                // }
                if (network->isReceptionThreadRunning()) {
                    network->generatePollFds();
                    if (acceptNewConnection)
                        checkForNewConnection(reinterpret_cast<cpn::Server*>(network), pollFds);
                    handleConnection(network, pollFds, acceptNewConnection);
                }
                LOG(network->getClassName() << "/TCP", "Poll fds: " << pollFds.size());
                for (size_t i = 0; i < pollFds.size(); ++i)
                    pollFds[i].revents = 0;
            }
        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            break;
        }
    }
    LOG(network->getClassName() << "/TCP", "Reception thread stopped!");
}
