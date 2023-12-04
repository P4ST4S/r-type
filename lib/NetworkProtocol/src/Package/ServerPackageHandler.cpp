/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** ServerPackageHandler
*/

#include "ServerPackageHandler.hpp"

ServerPackageHandler::ServerPackageHandler()
{
    this->_response = this->createPackage(RESPONSE);
}

ServerPackageHandler::~ServerPackageHandler()
{
}

bool ServerPackageHandler::start(uint16_t port) noexcept
{
    return (this->_server.start(port));
}

bool ServerPackageHandler::stop(void) noexcept
{
    return (this->_server.stop());
}

bool ServerPackageHandler::removeConnection(cpn::connectionID_t cid) noexcept
{
    return (this->_server.removeConnection(cid));
}

void ServerPackageHandler::setMaxConnection(ssize_t maxConnection) noexcept
{
    this->_server.setMaxConnection(maxConnection);
}

void ServerPackageHandler::setLock(bool lock) noexcept
{
    this->_server.setLock(lock);
}

bool ServerPackageHandler::getLock(void) const noexcept
{
    return (this->_server.getLock());
}

void ServerPackageHandler::dataToSendWhenRemove(std::deque<uint8_t> data) noexcept
{
    this->_server.dataToSendWhenRemove(data);
}

void ServerPackageHandler::dataToSendWhenLock(std::deque<uint8_t> data) noexcept
{
    this->_server.dataToSendWhenLock(data);
}

void ServerPackageHandler::dataToSendWhenMaxConnection(std::deque<uint8_t> data) noexcept
{
    this->_server.dataToSendWhenMaxConnection(data);
}

void ServerPackageHandler::dataToSendWhenConnectionNotConnected(std::deque<uint8_t> data) noexcept
{
    this->_server.dataToSendWhenConnectionNotConnected(data);
}

void ServerPackageHandler::receiveNewData(void) noexcept
{
    std::deque<Package> tmpPkgs;
    std::deque<cpn::connectionID_t> cids = this->_server.getConnectionList();

    while (!cids.empty()) {
        cpn::connectionData_t &connectionData = this->_server.getConnection(cids.front());

        // Check if connection is registered and add it if not
        if (this->_clients.find(cids.front()) == this->_clients.end()) {
            if (connectionData.connected) {
                this->_clients.emplace(cids.front()._id, ClientPackages_t(connectionData, this->_pkgFactory.getBodyFactory()));
                this->_newlyConnectedClients.push_back(cids.front());
            } else {
                cids.pop_front();
                continue;
            }
        }
        // Check if connection is connected and then write it's new data or removes it.
        try {
            if (connectionData.connected) {
                this->_clients.at(cids.front()._id)._tcpPkgParser.writeBytesIn(this->_server.receive(CPN_TCP, cids.front()));
                this->_clients.at(cids.front()._id)._udpPkgParser.writeBytesIn(this->_server.receive(CPN_UDP, cids.front()));
                tmpPkgs = this->_clients.at(cids.front()._id)._tcpPkgParser.getPackages();
                this->_clients.at(cids.front()._id)._pkgs.insert(this->_clients.at(cids.front()._id)._pkgs.end(), tmpPkgs.begin(), tmpPkgs.end());
                tmpPkgs = this->_clients.at(cids.front()._id)._udpPkgParser.getPackages();
                this->_clients.at(cids.front()._id)._pkgs.insert(this->_clients.at(cids.front()._id)._pkgs.end(), tmpPkgs.begin(), tmpPkgs.end());
            } else {
                this->_clients.erase(cids.front()._id);
                this->_disconnectedClients.push_back(cids.front());
            }
            // std::cout << cids.front() << ": " << std::endl;
            // for (size_t i = 0; i < this->_clients.at(cids.front()._id)._pkgs.size(); i++) {
            //     std::cout << "\t - " << this->_clients.at(cids.front()._id)._pkgs[i].getHeader().get()._commandType << std::endl;
            // }
        } catch (const std::out_of_range &e) {
            // std::cerr << "Error: " << e.what() << std::endl;
        }
        cids.pop_front();
    }
    this->_server.removeAllDisconnectedConnection();
}

std::deque<cpn::connectionID_t> ServerPackageHandler::getNewlyConnectedClients(void) noexcept
{
    std::deque<cpn::connectionID_t> tmp = this->_newlyConnectedClients;

    this->_newlyConnectedClients.clear();
    return (std::move(tmp));
}

std::deque<cpn::connectionID_t> ServerPackageHandler::getDisconnectedClients(void) noexcept
{
    std::deque<cpn::connectionID_t> tmp = this->_disconnectedClients;

    this->_disconnectedClients.clear();
    return (std::move(tmp));
}



Package ServerPackageHandler::createPackage(CommandType_t commandType)
{
    return (std::move(this->_pkgFactory.createPackage(commandType)));
}

std::deque<cpn::connectionID_t> ServerPackageHandler::getConnectionList(void) noexcept
{
    return (std::move(this->_server.getConnectionList()));
}

std::deque<Package> &ServerPackageHandler::accessPackages(cpn::connectionID_t cid)
{
    return (this->_clients.at(cid._id)._pkgs);
}

ssize_t ServerPackageHandler::sendPackage(int protocol, cpn::connectionID_t cid, const Package &pkg) const
{
    return (this->_server.send(protocol, cid, pkg.serialize()));
}

std::deque<ssize_t> ServerPackageHandler::sendPackage(int protocol, const std::deque<cpn::connectionID_t> &cid, const Package &pkg) const
{
    return (std::move(this->_server.multiSend(protocol, cid, pkg.serialize())));
}

ssize_t ServerPackageHandler::sendPackages(int protocol, cpn::connectionID_t cid, const std::deque<Package> &pkgs) const
{
    std::deque<uint8_t> data;

    for (auto &pkg : pkgs)
        data.insert(data.end(), pkg.serialize().begin(), pkg.serialize().end());
    return (this->_server.send(protocol, cid, data));
}

std::deque<ssize_t> ServerPackageHandler::sendPackages(int protocol, const std::deque<cpn::connectionID_t> &cid, const std::deque<Package> &pkgs) const
{
    std::deque<uint8_t> data;

    for (auto &pkg : pkgs)
        data.insert(data.end(), pkg.serialize().begin(), pkg.serialize().end());
    return (std::move(this->_server.multiSend(protocol, cid, data)));
}

ssize_t ServerPackageHandler::sendBodylessPackage(int protocol, cpn::connectionID_t cid, CommandType_t commandType)
{
    this->_bodylessHeader.get()._bodySize = 0;
    this->_bodylessHeader.get()._commandType = commandType;
    return (this->_server.send(protocol, cid, this->_bodylessHeader.serialize()));
}

std::deque<ssize_t> ServerPackageHandler::sendBodylessPackage(int protocol, const std::deque<cpn::connectionID_t> &cid, CommandType_t commandType)
{
    this->_bodylessHeader.get()._bodySize = 0;
    this->_bodylessHeader.get()._commandType = commandType;
    return (std::move(this->_server.multiSend(protocol, cid, this->_bodylessHeader.serialize())));
}

ssize_t ServerPackageHandler::sendResponse(int protocol, cpn::connectionID_t cid, CommandType_t commandType, ResponseStatus_t status)
{
    Body<Response_t> &body = this->_response.getBody<Body<Response_t>>();
    body.get()._commandType = commandType;
    body.get()._responseStatus = status;
    return (this->_server.send(protocol, cid, this->_response.serialize()));
}

std::deque<ssize_t> ServerPackageHandler::sendResponse(int protocol, const std::deque<cpn::connectionID_t> &cid, CommandType_t commandType, ResponseStatus_t status)
{
    Body<Response_t> &body = this->_response.getBody<Body<Response_t>>();
    body.get()._commandType = commandType;
    body.get()._responseStatus = status;
    return (std::move(this->_server.multiSend(protocol, cid, this->_response.serialize())));
}

cpn::Server &ServerPackageHandler::server(void) noexcept
{
    return (this->_server);
}

void ServerPackageHandler::clearAllPackages(void) noexcept
{
    std::unordered_map<uint64_t, ClientPackages_t>::iterator it = this->_clients.begin();

    for (; it != this->_clients.end(); ++it)
        it->second._pkgs.clear();
}
