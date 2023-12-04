/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** NewEntityEvent
*/

#include "NewEntityEvent.hpp"

NewEntityEvent::NewEntityEvent()
{
}


NewEntityEvent::NewEntityEvent(size_t entityId, size_t _entityType)
{
    this->setEntityId(entityId);
    this->setEntityType(_entityType);
}

NewEntityEvent::~NewEntityEvent()
{
}

size_t NewEntityEvent::getEntityId() const
{
    return this->_entityId;
}

size_t NewEntityEvent::getEntityType() const
{
    return this->_entityType;
}

void NewEntityEvent::setEntityId(size_t entityId)
{
    this->_entityId = entityId;
}

void NewEntityEvent::setEntityType(size_t entityType)
{
    this->_entityType = entityType;
}


Entity NewEntityEvent::createNewEntity(Registry &registry, size_t entityType)
{
    Entity entity = registry.spawnEntity();

    registry.addEvent<NewEntityEvent>(NewEntityEvent(entity, entityType));

    registry.emplaceComponent<VelocityComponent>(entity, Vector2D<double>(50, 50));
    InterpolationComponent &tmp = *registry.emplaceComponent<InterpolationComponent>(entity, InterpolationComponent());

    tmp.setPosition(0, 0);
    // registry.emplaceComponent<HitboxComponent>(entity, PLAYER_HITBOX);
    // registry.emplaceComponent<SpaceshipComponent>(entity, std::make_shared<PlayerSpaceship>(PlayerSpaceship()));
    // registry.addComponent<BasicWeaponComponent>(entity, BasicWeaponComponent(10, true));

    // registry.emplaceComponent<HealthComponent>(entity, 1);

    return (entity);
}
