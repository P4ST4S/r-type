/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** HandleAuthentication
*/

#include "HandleAuthenticationSystem.hpp"

HandleAuthenticationSystem::HandleAuthenticationSystem(ServerPackageHandler &serverPackageHandler, ClientHandler &clientHandler) :
_pkgHandler(serverPackageHandler), _cliHandler(clientHandler)
{
}

HandleAuthenticationSystem::~HandleAuthenticationSystem()
{
}

void HandleAuthenticationSystem::operator()(Registry &registry)
{
    // std::cout << "HANDLE AUTHENTICATION SYSTEM" << std::endl;
    bool itReset = false;
    std::deque<cpn::connectionID_t> tmpCids;
    std::unordered_map<uint64_t, ClientInfo_t>::iterator it = this->_cliHandler.get()[TARGET_CLI_UNKOWN].begin();
    Package pkg;
    CommandType_t cmdType;
    cpn::connectionID_t cid;

    while (it != this->_cliHandler.get()[TARGET_CLI_UNKOWN].end()) {
        itReset = false;
        cid = it->first;
        std::deque<Package> &pkgs = this->_pkgHandler.accessPackages(cid);

        while (!pkgs.empty()) {
            pkg = pkgs.front();
            pkgs.pop_front();
            cmdType = pkg.getHeader().get()._commandType;
            if (cmdType == CLIENT_CONNECTION_REQUEST) {
                Body<ClientConnectionRequest_t> &body = pkg.getBody<Body<ClientConnectionRequest_t>>();
                if (body.get()._clientType == CLI_TYPE_PLAYER &&
                this->_cliHandler.get()[TARGET_CLI_AUTH].size() < this->_cliHandler.getMaxAuthClients()) {
                    this->_cliHandler.moveClientTo(cid, TARGET_CLI_AUTH);
                    this->_pkgHandler.sendResponse(CPN_TCP, cid, cmdType, RESP_OK);
                    it = this->_cliHandler.get()[TARGET_CLI_UNKOWN].begin();
                    tmpCids = this->_cliHandler.generateCidList(TARGET_CLI_AUTH | TARGET_CLI_SPECTATOR);
                    this->_pkgHandler.sendPackage(CPN_TCP, tmpCids, this->_cliHandler.createPlayerList());
                    itReset = true;
                    break;
                } else if (body.get()._clientType == CLI_TYPE_SPECTATOR) {
                    this->_cliHandler.moveClientTo(cid, TARGET_CLI_SPECTATOR);
                    this->_pkgHandler.sendResponse(CPN_TCP, cid, cmdType, RESP_OK);
                    it = this->_cliHandler.get()[TARGET_CLI_UNKOWN].begin();
                    itReset = true;
                } else {
                    this->_pkgHandler.sendResponse(CPN_TCP, cid, cmdType, RESP_SERVER_FULL);
                }
            } else {
                this->_pkgHandler.sendResponse(CPN_TCP, cid, cmdType, RESP_KO);
            }
        }
        if (!itReset)
            ++it;
    }
}
