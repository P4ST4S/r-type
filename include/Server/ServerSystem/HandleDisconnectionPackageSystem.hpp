/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** HandleDisconnectionPackageSystem
*/

#pragma once

#include "Registry.hpp"
#include "Protocol.hpp"
#include "RTypeServerStruct.hpp"
#include "ClientHandler.hpp"
#include "ASystem.hpp"

class HandleDisconnectionPackageSystem : public ASystem {
public:
    HandleDisconnectionPackageSystem(ServerPackageHandler &serverPackageHandler, ClientHandler &clientHandler);
    ~HandleDisconnectionPackageSystem();

    void operator()(Registry &registry);

protected:
    ServerPackageHandler &_pkgHandler;
    ClientHandler &_cliHandler;
private:
};
