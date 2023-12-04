/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** HandleLoadStatusSystem
*/

#pragma once

#include "ASystem.hpp"
#include "Registry.hpp"
#include "Protocol.hpp"
#include "GameEngine.hpp"
#include "RTypeServerStruct.hpp"
#include "ClientHandler.hpp"
#include "PlayerComponent.hpp"
#include "MobComponent.hpp"
#include "WeaponComponent.hpp"
#include "RTypeServerStruct.hpp"
#include "ServerEvents.hpp"

class HandleLoadStatusSystem : public ASystem {
public:
    HandleLoadStatusSystem(ServerPackageHandler &serverPackageHandler, ClientHandler &clientHandler, serverState_t &state);
    ~HandleLoadStatusSystem();

    void operator()(Registry &registry);

    bool allClientReady(void);

protected:
    ServerPackageHandler &_pkgHandler;
    ClientHandler &_cliHandler;
    serverState_t &_state;
    bool _waitForClient = false;
private:
};
