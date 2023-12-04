/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** ClientPackageHandler
*/

#include "ClientPackageHandler.hpp"

ClientPackageHandler::ClientPackageHandler() :
_client(), _tcpPkgParser(_pkgFactory.getBodyFactory()), _udpPkgParser(_pkgFactory.getBodyFactory())
{
    this->_responsePkg = this->_pkgFactory.createPackage(RESPONSE);
}

ClientPackageHandler::ClientPackageHandler(const std::string &ip, uint16_t port) :
_client(ip, port), _tcpPkgParser(_pkgFactory.getBodyFactory()), _udpPkgParser(_pkgFactory.getBodyFactory())
{
    this->_responsePkg = this->_pkgFactory.createPackage(RESPONSE);
}

ClientPackageHandler::~ClientPackageHandler()
{
}

bool ClientPackageHandler::connectTo(const std::string &ip, uint16_t port) noexcept
{
    return (this->_client.connectTo(ip, port));
}

bool ClientPackageHandler::disconnect() noexcept
{
    return (this->_client.disconnect());
}

bool ClientPackageHandler::isConnected() const noexcept
{
    return (this->_client.isConnected());
}

void ClientPackageHandler::receiveNewData(void) noexcept
{
    std::deque<uint8_t> data;
    std::deque<Package> tmpPkgs;

    if (!this->isConnected())
        return;

    data = this->_client.receive(CPN_TCP);
    this->_tcpPkgParser.writeBytesIn(data);
    data = this->_client.receive(CPN_UDP);
    this->_udpPkgParser.writeBytesIn(data);

    tmpPkgs = this->_tcpPkgParser.getPackages();
    this->_pkgs.insert(this->_pkgs.end(), tmpPkgs.begin(), tmpPkgs.end());
    tmpPkgs = this->_udpPkgParser.getPackages();
    this->_pkgs.insert(this->_pkgs.end(), tmpPkgs.begin(), tmpPkgs.end());
}

std::deque<Package> ClientPackageHandler::getPackages(size_t nbPkgs) noexcept
{
    std::deque<Package> pkgs;

    if (nbPkgs == 0)
        nbPkgs = this->_pkgs.size();
    for (size_t i = 0; i < nbPkgs && !this->_pkgs.empty(); ++i) {
        pkgs.push_back(this->_pkgs.front());
        this->_pkgs.pop_front();
    }
    return (std::move(pkgs));
}

std::deque<Package> &ClientPackageHandler::accessPackages(void) noexcept
{
    return (this->_pkgs);
}

Package ClientPackageHandler::createPackage(CommandType_t commandType)
{
    return (std::move(this->_pkgFactory.createPackage(commandType)));
}

ssize_t ClientPackageHandler::sendPackage(int protocol, const Package &pkg) const
{
    return (this->_client.send(protocol, pkg.serialize()));
}

ssize_t ClientPackageHandler::sendPackages(int protocol, const std::deque<Package> &pkgs) const
{
    ssize_t size = 0;
    ssize_t totalSize = 0;
    std::deque<Package>::const_iterator it = pkgs.begin();
    std::deque<Package>::const_iterator itEnd = pkgs.end();

    for (; it != itEnd; ++it) {
        size = this->_client.send(protocol, it->serialize());
        if (size == -1) {
            if (totalSize == 0)
                return (-1);
            return (totalSize);
        }
        totalSize += size;
    }
    return (totalSize);
}

ssize_t ClientPackageHandler::sendBodylessPackage(int protocol, CommandType_t commandType)
{
    this->_bodylessPkg.get()._bodySize = 0;
    this->_bodylessPkg.get()._commandType = commandType;
    return (this->_client.send(protocol, this->_bodylessPkg.serialize()));
}

ssize_t ClientPackageHandler::sendResponse(int protocol, CommandType_t commandType, ResponseStatus_t status)
{
    Body<Response_t> &body = this->_responsePkg.getBody<Body<Response_t>>();

    body.get()._commandType = commandType;
    body.get()._responseStatus = status;
    return (this->_client.send(protocol, this->_responsePkg.serialize()));
}

void ClientPackageHandler::sendPackageToQueue(int protocol, const Package &pkg) noexcept
{
    this->writeInQueue(protocol, pkg.serialize());
}

void ClientPackageHandler::sendPackagesToQueue(int protocol, const std::deque<Package> &pkgs) noexcept
{
    std::deque<Package>::const_iterator it = pkgs.begin();
    std::deque<Package>::const_iterator itEnd = pkgs.end();

    for (; it != itEnd; ++it)
        this->writeInQueue(protocol, it->serialize());
}

void ClientPackageHandler::sendBodylessPackageToQueue(int protocol, CommandType_t commandType) noexcept
{
    this->_bodylessPkg.get()._bodySize = 0;
    this->_bodylessPkg.get()._commandType = commandType;
    this->writeInQueue(protocol, this->_bodylessPkg.serialize());
}

void ClientPackageHandler::sendResponseToQueue(int protocol, CommandType_t commandType, ResponseStatus_t status) noexcept
{
    Body<Response_t> &body = this->_responsePkg.getBody<Body<Response_t>>();

    body.get()._commandType = commandType;
    body.get()._responseStatus = status;
    this->writeInQueue(protocol, this->_responsePkg.serialize());
}

void ClientPackageHandler::sendQueuedPackages(int protocol)
{
    if (protocol & CPN_TCP) {
        this->_client.send(protocol, this->_tcpQueue.get());
        this->_tcpQueue.clear();
    }
    if (protocol & CPN_UDP) {
        this->_client.send(protocol, this->_udpQueue.get());
        this->_udpQueue.clear();
    }
}

void ClientPackageHandler::writeInQueue(int protocol, const std::deque<uint8_t> &data) noexcept
{
    if (protocol & CPN_TCP)
        this->_tcpPkgParser.writeBytesIn(data);
    if (protocol & CPN_UDP)
        this->_udpPkgParser.writeBytesIn(data);
}
