/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** BodyFactory
*/

#include "BodyFactory.hpp"

BodyFactory::BodyFactory()
{
    this->_bodies.resize(TOTAL_COMMANDS);

    this->_bodies[RESPONSE].reset(new Body<Response_t>);
    this->_bodies[CLIENT_CONNECTION_REQUEST].reset(new Body<ClientConnectionRequest_t>);
    this->_bodies[CLIENT_SERVER_CONNECT].reset(new Body<ClientServerConnect_t>);
    this->_bodies[PING].reset(new EmptyBody);
    this->_bodies[CLIENT_DISCONNECT].reset(new Body<ClientDisconnect_t>);
    this->_bodies[STOP_SERVER].reset(new EmptyBody);

    this->_bodies[SERVER_SETTINGS].reset(new Body<ServerSettings_t>);
    this->_bodies[LOCAL_SERVER_SETTINGS].reset(new Body<ServerSettings_t>);
    this->_bodies[SET_USERNAME].reset(new Body<SetUsername_t>);
    this->_bodies[PLAYER_LIST].reset(new DynamicBody<PlayerList_t>);
    this->_bodies[SET_READY_STATUS].reset(new Body<SetReadyStatus_t>);
    this->_bodies[START_GAME_LOADING].reset(new EmptyBody);
    this->_bodies[GAME_START].reset(new EmptyBody);

    this->_bodies[NEW_ENTITIES].reset(new DynamicBody<NewEntities_t>);
    this->_bodies[MOVE_ENTITIES].reset(new HybridBody<MoveEntitiesSta_t, MoveEntitiesDyn_s>);
    this->_bodies[DESTROY_ENTITIES].reset(new DynamicBody<DestroyEntities_t>);
    this->_bodies[CLIENT_MOVE_EVENT].reset(new Body<ClientMoveEvent_t>);
    this->_bodies[CLIENT_SHOOT_EVENT].reset(new Body<ClientShootEvent_t>);
    this->_bodies[ENTITY_ACTION].reset(new Body<EntityAction_t>);
    this->_bodies[END_GAME].reset(new Body<EndGame_t>);
}

BodyFactory::~BodyFactory()
{
}

IBody *BodyFactory::createBody(enum CommandType_u type)
{
    if (type == UNKNOWN)
        return (this->_emptyBody->clone());
    if (type >= TOTAL_COMMANDS)
        return (nullptr);
    return (this->_bodies[type]->clone());
}

size_t BodyFactory::getBodySize(enum CommandType_u type)
{
    if (type == UNKNOWN)
        return (this->_emptyBody->size());
    if (type >= TOTAL_COMMANDS)
        return (0);
    return (this->_bodies[type]->size());
}

bool BodyFactory::bodyExist(enum CommandType_u type)
{
    if (type == UNKNOWN || type < TOTAL_COMMANDS)
        return (true);
    return (false);
}
