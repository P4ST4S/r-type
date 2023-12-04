/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** HandleDisconnectionPackage
*/

#include "HandleDisconnectionPackageSystem.hpp"

HandleDisconnectionPackageSystem::HandleDisconnectionPackageSystem(ServerPackageHandler &serverPackageHandler, ClientHandler &clientHandler) :
_pkgHandler(serverPackageHandler), _cliHandler(clientHandler)
{
}

HandleDisconnectionPackageSystem::~HandleDisconnectionPackageSystem()
{
}

void HandleDisconnectionPackageSystem::operator()(Registry &registry)
{
    // std::cout << "HANDLE DISCONNECTION PACKAGE SYSTEM" << std::endl;
    bool clientBeenRemoved = false;
    Package pkg = this->_pkgHandler.createPackage(CLIENT_DISCONNECT);
    std::deque<cpn::connectionID_t> cids = this->_pkgHandler.getConnectionList();
    std::deque<Package> pkgs;
    size_t entityId;

    for (size_t i = 0; i < cids.size(); ++i) {
        pkgs = PackageParser::extractPackages(this->_pkgHandler.accessPackages(cids[i]), {
            CLIENT_DISCONNECT
        });
        if (!pkgs.empty()) {
            pkg.getBody<Body<ClientDisconnect_t>>().get()._clientId = this->_cliHandler.getClientInfo(cids[i]).entityId;
            this->_pkgHandler.sendResponse(CPN_TCP, cids[i], CLIENT_DISCONNECT, RESP_KICKED_FROM_SERVER);
            this->_pkgHandler.removeConnection(cids[i]);
            if (this->_cliHandler.getClientType(cids[i]) == TARGET_CLI_AUTH) {
                clientBeenRemoved = true;
                this->_pkgHandler.sendPackage(CPN_TCP, this->_cliHandler.generateCidList(TARGET_CLI_AUTH | TARGET_CLI_SPECTATOR), pkg);
            }
        }
    }
    if (clientBeenRemoved)
        this->_pkgHandler.sendPackage(CPN_TCP, this->_cliHandler.generateCidList(TARGET_CLI_AUTH | TARGET_CLI_SPECTATOR), this->_cliHandler.createPlayerList());
}
