/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** BodyStruct
*/

#pragma once
// #pragma pack(1)

#include "CommandTypes.hpp"

#include <cstdint>

#define SIZE_OF_USERNAME 16

typedef enum ResponseStatus_u : uint8_t {
    RESP_OK,
    RESP_KO,
    RESP_INVALID_HEADER,
    RESP_INVALID_BODY,
    RESP_UNAUTHORIZED,
    RESP_SERVER_FULL,
    RESP_SERVER_CLOSING,
    RESP_KICKED_FROM_SERVER,
    RESP_INFORMATION,
    TOTAL_RESP,
    RESP_UNKNOWN = 0xFF
} ResponseStatus_t;

typedef enum ClientType_u : uint8_t {
    CLI_TYPE_PLAYER,
    CLI_TYPE_SPECTATOR,
    TOTAL_CLIENT_TYPE
} ClientType_t;

#define RESP_CAST(uint8Code) static_cast<ResponseStatus_t>(uint8Code)

typedef struct Response_s {
    uint8_t _responseStatus; /**< The response status code. */
    uint16_t _commandType;  /**< The associated command type. */
} Response_t;

typedef struct ClientConnectionRequest_s {
    uint8_t _clientType; /**< The type of client (e.g., player, spectator). */
} ClientConnectionRequest_t;

typedef struct ClientDisconnect_s {
    uint16_t _clientId; /**< The type of client (e.g., player, spectator). */
} ClientDisconnect_t;




typedef struct ClientServerConnect_s {
    uint32_t _ip;   /**< The IP address of the client. */
    uint16_t _port; /**< The port on which the client listens. */
} ClientServerConnect_t;

typedef struct ServerSettings_s {
    uint32_t _localIp;        /**< Local IP address of the server. */
    uint32_t _publicIp;       /**< Public IP address of the server. */
    uint16_t _port;           /**< Port on which the server listens. */
    uint8_t _serverMode;      /**< Server mode (HOST or CLIENT). */
    uint8_t _status;          /**< Server status (e.g., lobby, in-game). */
    uint8_t _maxClients;      /**< Maximum number of clients that can be connected. */
    uint8_t _friendlyFire;    /**< Flag indicating if friendly fire is enabled. */
    uint8_t _enemyCollision;  /**< Flag indicating if player collides with enemies. */
    uint8_t _allyCollision;   /**< Flag indicating if player collides with allies. */
} ServerSettings_t;

typedef struct SetUsername_s {
    char _username[SIZE_OF_USERNAME];     /**< The username of the client. */
} SetUsername_t;

typedef struct PlayerList_s {
    uint16_t _playerId;
    char _username[SIZE_OF_USERNAME];
    bool _ready;
} PlayerList_t;

typedef struct SetReadyStatus_s {
    bool _readyStatus;
} SetReadyStatus_t;

typedef struct NewEntities_s {
    uint16_t _id;
    uint8_t _type;
} NewEntities_t;

typedef struct MoveEntitiesSta_s {
    double _timeStamp;
} MoveEntitiesSta_t;

typedef struct MoveEntitiesDyn_s {
    uint16_t _id;
    int16_t _x;
    int16_t _y;
    int16_t _vx;
    int16_t _vy;
} MoveEntitiesDyn_t;

typedef struct EntityAction_s {
    uint16_t _id;
    uint8_t _action;
} EntityAction_t;

typedef struct DestroyEntities_s {
    uint16_t _id;
} DestroyEntities_t;

typedef struct ClientMoveEvent_s {
    bool _up;
    bool _down;
    bool _left;
    bool _right;
} ClientMoveEvent_t;

typedef struct ClientShootEvent_s {
    bool _shoot;
} ClientShootEvent_t;

typedef struct EndGame_s {
    bool _win;
} EndGame_t;
