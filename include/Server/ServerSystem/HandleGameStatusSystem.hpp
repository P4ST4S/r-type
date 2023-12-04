/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** HandleGameStatusSystem
*/

#pragma once

#include "Registry.hpp"
#include "Protocol.hpp"
#include "RTypeServerStruct.hpp"
#include "ClientHandler.hpp"
#include "ASystem.hpp"
#include "RTypeServerStruct.hpp"
#include "ServerEvents.hpp"
#include "PartyEndEvent.hpp"

class HandleGameStatusSystem : public ASystem {
public:
    HandleGameStatusSystem(ServerPackageHandler &serverPackageHandler, ClientHandler &clientHandler, serverState_t &state);
    ~HandleGameStatusSystem();

    void operator()(Registry &registry);
    void handleMovementShootPackage(Registry &registry);

protected:
    ServerPackageHandler &_pkgHandler;
    ClientHandler &_cliHandler;
    serverState_t &_state;
    std::time_t _time;
    bool firstTick = true;
private:
};
