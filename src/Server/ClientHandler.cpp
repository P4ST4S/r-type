/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** ClientFilter
*/

#include "ClientHandler.hpp"

ClientHandler::ClientHandler(ServerPackageHandler &serverPackageHandler) :
_pkgHandler(serverPackageHandler)
{
    this->_clients[TARGET_CLI_UNKOWN];
    this->_clients[TARGET_CLI_AUTH];
    this->_clients[TARGET_CLI_SPECTATOR];
}

ClientHandler::~ClientHandler()
{
}

std::unordered_map<int, std::unordered_map<uint64_t, ClientInfo_t>> &ClientHandler::get(void)
{
    return (this->_clients);
}

void ClientHandler::addClient(cpn::connectionID_t cid)
{
    std::string name = "Player_" + std::to_string(cid._id);
    name.resize(SIZE_OF_USERNAME);
    this->_clients[TARGET_CLI_UNKOWN][cid] = {cid, name, false, 0, false, false};
}

void ClientHandler::removeClient(cpn::connectionID_t cid)
{
    if (this->_clients[TARGET_CLI_UNKOWN].find(cid) != this->_clients[TARGET_CLI_UNKOWN].end())
        this->_clients[TARGET_CLI_UNKOWN].erase(cid);
    if (this->_clients[TARGET_CLI_AUTH].find(cid) != this->_clients[TARGET_CLI_AUTH].end())
        this->_clients[TARGET_CLI_AUTH].erase(cid);
    if (this->_clients[TARGET_CLI_SPECTATOR].find(cid) != this->_clients[TARGET_CLI_SPECTATOR].end())
        this->_clients[TARGET_CLI_SPECTATOR].erase(cid);
}

ClientInfo_t &ClientHandler::getClientInfo(cpn::connectionID_t cid)
{
    return (this->_clients.at(this->getClientType(cid)).at(cid));
}

int ClientHandler::getClientType(cpn::connectionID_t cid)
{
    int targetType = TARGET_CLI_UNKOWN;

    if (this->_clients[TARGET_CLI_UNKOWN].find(cid) != this->_clients[TARGET_CLI_UNKOWN].end())
        targetType = TARGET_CLI_UNKOWN;
    if (this->_clients[TARGET_CLI_SPECTATOR].find(cid) != this->_clients[TARGET_CLI_SPECTATOR].end())
        targetType = TARGET_CLI_SPECTATOR;
    if (this->_clients[TARGET_CLI_AUTH].find(cid) != this->_clients[TARGET_CLI_AUTH].end())
        targetType = TARGET_CLI_AUTH;
    return (targetType);
}

std::deque<cpn::connectionID_t> ClientHandler::generateCidList(int targetType)
{
    std::deque<cpn::connectionID_t> cids;
    std::unordered_map<uint64_t, ClientInfo_t>::iterator it;
    std::unordered_map<uint64_t, ClientInfo_t>::iterator itEnd;

    if (targetType & TARGET_CLI_UNKOWN) {
        it = this->_clients[TARGET_CLI_UNKOWN].begin();
        itEnd = this->_clients[TARGET_CLI_UNKOWN].end();
        for (; it != itEnd; it++)
            cids.push_back(it->first);
    }
    if (targetType & TARGET_CLI_AUTH) {
        it = this->_clients[TARGET_CLI_AUTH].begin();
        itEnd = this->_clients[TARGET_CLI_AUTH].end();
        for (; it != itEnd; it++)
            cids.push_back(it->first);
    }
    if (targetType & TARGET_CLI_SPECTATOR) {
        it = this->_clients[TARGET_CLI_SPECTATOR].begin();
        itEnd = this->_clients[TARGET_CLI_SPECTATOR].end();
        for (; it != itEnd; it++)
            cids.push_back(it->first);
    }
    return (std::move(cids));
}

void ClientHandler::moveClientTo(cpn::connectionID_t cid, int targetType)
{
    ClientInfo_t clientInfo = this->_clients[this->getClientType(cid)][cid];

    this->_clients[this->getClientType(cid)].erase(cid);
    this->_clients[targetType][cid] = clientInfo;
}

Package ClientHandler::createPlayerList(void)
{
    Package pkg = this->_pkgHandler.createPackage(PLAYER_LIST);
    PlayerList_t playerList;
    DynamicBody<PlayerList_t> &body = pkg.getBody<DynamicBody<PlayerList_t>>();
    std::deque<cpn::connectionID_t> cids = this->generateCidList(TARGET_CLI_AUTH);

    for (cpn::connectionID_t &cid : cids) {
        playerList._playerId = this->_clients[TARGET_CLI_AUTH][cid].entityId;
        playerList._ready = this->_clients[TARGET_CLI_AUTH][cid].ready;
        memcpy(playerList._username, this->_clients[TARGET_CLI_AUTH][cid].name.c_str(), SIZE_OF_USERNAME);
        body.add(playerList);
    }
    return (std::move(pkg));
}

void ClientHandler::setMaxAuthClients(size_t maxAuthClients) noexcept
{
    this->_maxAuthClients = maxAuthClients;
}
size_t ClientHandler::getMaxAuthClients(void) const noexcept
{
    return (this->_maxAuthClients);
}
