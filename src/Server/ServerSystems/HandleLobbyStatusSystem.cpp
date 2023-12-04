/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** HandleLobbyStatusSystem
*/

#include "HandleLobbyStatusSystem.hpp"

HandleLobbyStatusSystem::HandleLobbyStatusSystem(ServerPackageHandler &serverPackageHandler, ClientHandler &clientHandler, serverState_t &state) :
_pkgHandler(serverPackageHandler), _cliHandler(clientHandler), _state(state)
{
}

HandleLobbyStatusSystem::~HandleLobbyStatusSystem()
{
}

void HandleLobbyStatusSystem::operator()(Registry &registry)
{
    // std::cout << "HANDLE LOBBY STATUS SYSTEM" << std::endl;
    if (this->_state == STATUS_LOBBY) {
        Package pkg;
        std::deque<Package> pkgs;
        cpn::connectionID_t cid;
        std::deque<cpn::connectionID_t> cids = this->_cliHandler.generateCidList(TARGET_CLI_AUTH);

        for (size_t i = 0; i < cids.size(); ++i) {
            cid = cids[i];
            pkgs = PackageParser::extractPackages(this->_pkgHandler.accessPackages(cid), {
                START_GAME_LOADING
            });
            while (!pkgs.empty()) {
                pkg = pkgs.front();
                pkgs.pop_front();
                switch (pkg.getHeader().get()._commandType) {
                    case START_GAME_LOADING:
                        this->_pkgHandler.sendBodylessPackage(CPN_TCP, this->_cliHandler.generateCidList(TARGET_CLI_AUTH | TARGET_CLI_SPECTATOR), START_GAME_LOADING);
                        this->_state = STATUS_LOAD;
                        break;
                }
            }
        }
    }
}
