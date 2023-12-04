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

class SendDestroyEntitiesPackageSystem : public ASystem {
public:
    SendDestroyEntitiesPackageSystem(ServerPackageHandler &serverPackageHandler, ClientHandler &clientHandler);
    ~SendDestroyEntitiesPackageSystem();

    void operator()(Registry &registry);
protected:
    ServerPackageHandler &_pkgHandler;
    ClientHandler &_cliHandler;
private:
};
