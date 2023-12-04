/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** KillEntityEvent
*/

#include "KillEntityEvent.hpp"

KillEntityEvent::KillEntityEvent()
{
}


KillEntityEvent::KillEntityEvent(size_t entityId)
{
    this->setEntityId(entityId);
}

KillEntityEvent::~KillEntityEvent()
{
}

size_t KillEntityEvent::getEntityId() const
{
    return this->_entityId;
}

void KillEntityEvent::setEntityId(size_t entityId)
{
    this->_entityId = entityId;
}

void KillEntityEvent::killEntity(Registry &registry, size_t entityId)
{
    registry.addEvent<KillEntityEvent>(KillEntityEvent(entityId));
    registry.killEntity(registry.entityFromIndex(entityId));
}
