/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** HandleConnectionSystem
*/

#pragma once

#include "Registry.hpp"
#include "Protocol.hpp"
#include "RTypeServerStruct.hpp"
#include "ClientHandler.hpp"
#include "ASystem.hpp"
#include "KillEntityEvent.hpp"

class HandleConnectionSystem : public ASystem {
public:
    HandleConnectionSystem(ServerPackageHandler &serverPackageHandler, ClientHandler &clientHandler);
    ~HandleConnectionSystem();

    void operator()(Registry &registry);

    bool handleNewConnection(void);
    bool handleDisconnection(Registry &registry);
protected:
    bool _connectionModified = false;
    ServerPackageHandler &_pkgHandler;
    ClientHandler &_cliHandler;
private:
};  