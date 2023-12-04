/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** SendMoveEntitiesPackageSystem
*/

#include "SendMoveEntitiesPackageSystem.hpp"

SendMoveEntitiesPackageSystem::SendMoveEntitiesPackageSystem(ServerPackageHandler &serverPackageHandler, ClientHandler &clientHandler, serverState_t &state) :
_pkgHandler(serverPackageHandler), _cliHandler(clientHandler), _state(state)

{
}

SendMoveEntitiesPackageSystem::~SendMoveEntitiesPackageSystem()
{
}


void SendMoveEntitiesPackageSystem::operator()(Registry &registry, SparseArray<InterpolationComponent> &interpolationComponents)
{
    // std::cout << "SEND MOVE ENTITIES PACKAGE SYSTEM" << std::endl;
    if (this->_state == STATUS_GAME) {
        MoveEntitiesDyn_t tmp;
        Package pkg = this->_pkgHandler.createPackage(MOVE_ENTITIES);
        HybridBody<MoveEntitiesSta_t, MoveEntitiesDyn_t> &body = pkg.getBody<HybridBody<MoveEntitiesSta_t, MoveEntitiesDyn_t>>();
        std::chrono::_V2::system_clock::time_point timeNow = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration;

        if (this->_prevState != this->_state)
            this->_timeStamp = timeNow;
        duration = timeNow - this->_timeStamp;

        body.get().first.get()._timeStamp = duration.count();
        for (size_t i = 0; i < interpolationComponents.size(); i++) {
            if (interpolationComponents[i].has_value()) {
                interpolationComponents[i].value().getMoveEntityBody(i, tmp);
                body.get().second.add(tmp);
            }
        }
        if (pkg.getBody<IBody>().size() > 0)
            this->_pkgHandler.sendPackage(CPN_UDP, this->_cliHandler.generateCidList(TARGET_CLI_AUTH | TARGET_CLI_SPECTATOR), pkg);

    }
    this->_prevState = this->_state;
    // std::cout << "SEND MOVE ENTITIES PACKAGE SYSTEM END" << std::endl;
}
