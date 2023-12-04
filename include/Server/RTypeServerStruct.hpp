/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** RTypeServerStruct
*/

#pragma once

#include "CrossPlatformNetwork.hpp"

#include <string>

#define TARGET_CLI_UNKOWN (1 << 0)
#define TARGET_CLI_AUTH (1 << 1)
#define TARGET_CLI_SPECTATOR (1 << 2)

typedef enum serverState_u {
    STATUS_LOBBY,
    STATUS_LOAD,
    STATUS_GAME,
    STATUS_UNLOAD,
    TOTAL_STATUS
} serverState_t;

typedef struct ClientInfo_s {
    cpn::connectionID_t cid;
    std::string name;
    bool entityCreated;
    std::size_t entityId;
    bool ready;
    bool readyToGame;
} ClientInfo_t;
