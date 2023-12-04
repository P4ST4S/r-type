/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** RTypeServer
*/

#pragma once

#include "RTypeServerStruct.hpp"
#include "ClientHandler.hpp"
#include "InputEventListener.hpp"
#include "PartyEndEvent.hpp"

#include "Protocol.hpp"
#include "Clock.hpp"

//Components and Systems
#include "GameEngine.hpp"
#include "ServerSystems.hpp"

#include "GameMacro.hpp"
#include "Registry.hpp"

#include <signal.h>
#include <unordered_map>
#include <vector>
#include <deque>

class RTypeServer {
public:

    RTypeServer();
    ~RTypeServer();

    void logServerInfo(void);

    bool run(uint16_t port);

protected:
    bool _running = false;
    Registry _registry;
    ServerPackageHandler _pkgHandler;
    ClientHandler _cliHandler;
    serverState_t _state = STATUS_LOBBY;
    Clock _clk;
    int _tickRate = 10;
private:
};
