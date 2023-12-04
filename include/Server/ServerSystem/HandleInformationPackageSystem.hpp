/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** HandleInformationPackageSystem
*/

#pragma once

#include "Registry.hpp"
#include "Protocol.hpp"
#include "RTypeServerStruct.hpp"
#include "ClientHandler.hpp"
#include "ASystem.hpp"

class HandleInformationPackageSystem : public ASystem {
public:
    HandleInformationPackageSystem(ServerPackageHandler &serverPackageHandler, ClientHandler &clientHandler);
    ~HandleInformationPackageSystem();

    void operator()(Registry &registry);

    bool handleNewConnection(void);
    bool handleDisconnection(void);
protected:
    bool _connectionModified = false;
    ServerPackageHandler &_pkgHandler;
    ClientHandler &_cliHandler;
private:
};
