/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** SendNewEntitiesPackageSystem
*/

#pragma once

#include "Registry.hpp"
#include "Protocol.hpp"
#include "RTypeServerStruct.hpp"
#include "ClientHandler.hpp"
#include "ServerEvents.hpp"
#include "ASystem.hpp"

class SendNewEntitiesPackageSystem : public ASystem {
public:
    SendNewEntitiesPackageSystem(ServerPackageHandler &serverPackageHandler, ClientHandler &clientHandler);
    ~SendNewEntitiesPackageSystem();

    void operator()(Registry &registry);
protected:
    ServerPackageHandler &_pkgHandler;
    ClientHandler &_cliHandler;
private:
};
