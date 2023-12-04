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

class HandleAuthenticationSystem : public ASystem {
public:
    HandleAuthenticationSystem(ServerPackageHandler &serverPackageHandler, ClientHandler &clientHandler);
    ~HandleAuthenticationSystem();

    void operator()(Registry &registry);

protected:
    ServerPackageHandler &_pkgHandler;
    ClientHandler &_cliHandler;
private:
};
