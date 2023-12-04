/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** HandleUnloadStatusSystem
*/

#include "HandleUnloadStatusSystem.hpp"

HandleUnloadStatusSystem::HandleUnloadStatusSystem(ServerPackageHandler &serverPackageHandler, ClientHandler &clientHandler, serverState_t &state) :
_pkgHandler(serverPackageHandler), _cliHandler(clientHandler), _state(state)
{
}

HandleUnloadStatusSystem::~HandleUnloadStatusSystem()
{
}

void HandleUnloadStatusSystem::operator()(Registry &registry)
{
    // std::cout << "HANDLE UNLOAD STATUS SYSTEM" << std::endl;
    if (this->_state == STATUS_UNLOAD) {
        std::deque<cpn::connectionID_t> cids = this->_cliHandler.generateCidList(TARGET_CLI_AUTH);
        while (!cids.empty()) {
            ClientInfo_t &cliInfo = this->_cliHandler.getClientInfo(cids.front());

            cliInfo.entityCreated = false;
            KillEntityEvent::killEntity(registry, cliInfo.entityId);

            cids.pop_front();
        }

        this->_state = STATUS_LOBBY;
        registry.killAllEntities();
        // this->_pkgHandler.setLock(false);
    }
}
