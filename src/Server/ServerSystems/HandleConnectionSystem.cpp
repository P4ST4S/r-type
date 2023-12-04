/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** HandleConnectionSystem
*/

#include "HandleConnectionSystem.hpp"

HandleConnectionSystem::HandleConnectionSystem(ServerPackageHandler &serverPackageHandler, ClientHandler &clientHandler) :
_pkgHandler(serverPackageHandler), _cliHandler(clientHandler)
{
}

HandleConnectionSystem::~HandleConnectionSystem()
{
}

void HandleConnectionSystem::operator()(Registry &registry)
{
    // std::cout << "HANDLE CONNECTION SYSTEM" << std::endl;
    this->_connectionModified = false;

    if (this->handleNewConnection())
        this->_connectionModified = true;
    if (this->handleDisconnection(registry))
        this->_connectionModified = true;

    if (this->_connectionModified)
        this->_pkgHandler.sendPackage(CPN_TCP, this->_cliHandler.generateCidList(TARGET_CLI_AUTH | TARGET_CLI_SPECTATOR), this->_cliHandler.createPlayerList());
}

bool HandleConnectionSystem::handleNewConnection(void)
{
    std::deque<cpn::connectionID_t> cids = this->_pkgHandler.getNewlyConnectedClients();

    for (cpn::connectionID_t &cid : cids) {
        this->_cliHandler.addClient(cid);
    }
    return (!cids.empty());
}

bool HandleConnectionSystem::handleDisconnection(Registry &registry)
{
    Package pkg = this->_pkgHandler.createPackage(CLIENT_DISCONNECT);
    int targetType = TARGET_CLI_UNKOWN;
    std::deque<cpn::connectionID_t> cids = this->_pkgHandler.getDisconnectedClients();
    std::deque<cpn::connectionID_t> tmpCids = this->_pkgHandler.getDisconnectedClients();
    std::deque<size_t> entityIds;

    for (cpn::connectionID_t &cid : cids) {
        targetType = this->_cliHandler.getClientType(cid);
        if (targetType == TARGET_CLI_AUTH) {
            entityIds.push_back(this->_cliHandler.getClientInfo(cid).entityId);
            if (this->_cliHandler.getClientInfo(cid).entityCreated)
                KillEntityEvent::killEntity(registry, entityIds.back());
        }
        this->_cliHandler.removeClient(cid._id);
    }
    tmpCids = this->_cliHandler.generateCidList(TARGET_CLI_AUTH | TARGET_CLI_SPECTATOR);
    for (size_t &id : entityIds) {
        pkg.getBody<Body<ClientDisconnect_t>>().get()._clientId = id;
        this->_pkgHandler.sendPackage(CPN_TCP, tmpCids, pkg);
    }
    return (!cids.empty());
}