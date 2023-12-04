/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** SendDestroyedEntitiesPackageSystem
*/

#pragma once

#include "Registry.hpp"
#include "Protocol.hpp"
#include "RTypeServerStruct.hpp"
#include "ClientHandler.hpp"
#include "ServerEvents.hpp"
#include "ASystem.hpp"

class SendMoveEntitiesPackageSystem : public ASystem {
public:
    SendMoveEntitiesPackageSystem(ServerPackageHandler &serverPackageHandler, ClientHandler &clientHandler, serverState_t &state);
    ~SendMoveEntitiesPackageSystem();

    void operator()(Registry &registry, SparseArray<InterpolationComponent> &interpolationComponents);
protected:
    ServerPackageHandler &_pkgHandler;
    ClientHandler &_cliHandler;
    serverState_t &_state;
    serverState_t _prevState;

    uint8_t _order1 = 0;
    uint8_t _order2 = 0;
    bool _pick = true;
    std::chrono::_V2::system_clock::time_point _timeStamp;

    // std::chrono::duration<size_t, std::nano> _duration = this->_gap - this->_prevGap;
private:
};
