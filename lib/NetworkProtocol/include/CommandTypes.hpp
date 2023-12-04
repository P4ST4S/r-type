/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** CommandTypes
*/

#pragma once

#include <stdint.h>

typedef enum CommandType_u : uint16_t {
    RESPONSE = 0x0000,
    CLIENT_CONNECTION_REQUEST,
    CLIENT_SERVER_CONNECT,
    PING,
    CLIENT_DISCONNECT,
    STOP_SERVER,

    SERVER_SETTINGS,
    LOCAL_SERVER_SETTINGS,
    SET_USERNAME,
    PLAYER_LIST,
    SET_READY_STATUS,
    START_GAME_LOADING,
    GAME_START,

    NEW_ENTITIES,
    MOVE_ENTITIES,
    DESTROY_ENTITIES,
    CLIENT_MOVE_EVENT,
    CLIENT_SHOOT_EVENT,
    ENTITY_ACTION,
    END_GAME,
    TOTAL_COMMANDS,

    UNKNOWN = 0xFFFF
} CommandType_t;

/**
 ** @brief Small macro to cast uint16_t into CommandType_u
 **
 ** @param uint16Nb Expecting an uint16_t typed value or equivalent.
 */
#define CMD_CAST(uint16Nb) (static_cast<CommandType_u>(uint16Nb))
