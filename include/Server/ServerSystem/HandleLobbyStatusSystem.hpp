/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** HandleAuthenticationSystem
*/

#pragma once

#include "Registry.hpp"
#include "Protocol.hpp"
#include "RTypeServerStruct.hpp"
#include "ClientHandler.hpp"
#include "ASystem.hpp"
#include "RTypeServerStruct.hpp"

class HandleLobbyStatusSystem : public ASystem {
public:
    HandleLobbyStatusSystem(ServerPackageHandler &serverPackageHandler, ClientHandler &clientHandler, serverState_t &state);
    ~HandleLobbyStatusSystem();

    void operator()(Registry &registry);

protected:
    ServerPackageHandler &_pkgHandler;
    ClientHandler &_cliHandler;
    serverState_t &_state;
private:
};
