/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** ReceiveNewDataSystem
*/

#pragma once

#include "Registry.hpp"
#include "Protocol.hpp"
#include "RTypeServerStruct.hpp"
#include "ClientHandler.hpp"
#include "ASystem.hpp"

class ReceiveNewDataSystem : public ASystem {
public:
    ReceiveNewDataSystem(ServerPackageHandler &serverPackageHandler);
    ~ReceiveNewDataSystem();

    void operator()(Registry &registry);

protected:
    ServerPackageHandler &_pkgHandler;
private:
};
