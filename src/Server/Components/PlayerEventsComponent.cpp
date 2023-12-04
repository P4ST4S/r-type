/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** PlayerEventsComponent
*/

#include "PlayerEventsComponent.hpp"

PlayerEventsComponent::PlayerEventsComponent()
{
    this->_events = {false, false, false, false, false};
}

PlayerEventsComponent::~PlayerEventsComponent()
{
}

void PlayerEventsComponent::updateEvent(const ClientMoveEvent_t &move)
{
    this->_events._up = move._up;
    this->_events._down = move._down;
    this->_events._left = move._left;
    this->_events._right = move._right;
}

void PlayerEventsComponent::updateEvent(const ClientShootEvent_t &shoot)
{
    this->_events._shoot = shoot._shoot;
}

const PlayerEvents_t &PlayerEventsComponent::getEvents(void) const
{
    return (this->_events);
}

void destroyPlayerEvents(Registry &registry, Entity const &entity)
{
    SparseArray<PlayerEventsComponent> &playerEventsArray = registry.getComponents<PlayerEventsComponent>();

    if (playerEventsArray.size() > entity)
        playerEventsArray[entity].reset();
}
