/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** WeaponComponent
*/

#include "WeaponComponent.hpp"

WeaponComponent::WeaponComponent(std::size_t damage, bool direction, int owner) : _damages(damage), _direction(direction), _owner(owner)
{
}

bool WeaponComponent::shoot(Registry &registry, Vector2D<double> position, int cooldown)
{
    if (this->_coolDown > 0)
        return false;

    Entity entity = registry.spawnEntity();
    int type = (MISSILE_ID << 4) | _owner;


    registry.addComponent<InterpolationComponent>(entity, InterpolationComponent(position));
    if (_direction == true)
        registry.addComponent<VelocityComponent>(entity, VelocityComponent(Vector2D<double>(100, 0)));
    else
        registry.addComponent<VelocityComponent>(entity, VelocityComponent(Vector2D<double>(-100, 0)));
    registry.addComponent<DamageHitboxComponent>(entity, DamageHitboxComponent(Vector2D<double>(50, 50), _damages));
    registry.addComponent<BulletComponent>(entity, BulletComponent());
    registry.addComponent<PositionComponent>(entity, PositionComponent(position));
    registry.addComponent<IdentityComponent>(entity, IdentityComponent(type));
    registry.addEvent<NewEntityEvent>(NewEntityEvent(entity, type));
    this->_coolDown = cooldown;
    return true;
}

void WeaponComponent::updateCooldown()
{
    if (this->_coolDown > 0)
        this->_coolDown--;
}

void destroyWeapon(Registry &registry, Entity const &entity)
{
    SparseArray<WeaponComponent> &weaponArray = registry.getComponents<WeaponComponent>();

    if (weaponArray.size() > entity)
        weaponArray[entity].reset();
}
