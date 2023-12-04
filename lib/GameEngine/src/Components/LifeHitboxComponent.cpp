/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** LifeHitboxComponent
*/

#include "LifeHitboxComponent.hpp"

LifeHitboxComponent::LifeHitboxComponent(double width, double height, std::size_t health) : _size(width, height), _health(health), _maxHealth(health), _touch(false)
{
}

LifeHitboxComponent::LifeHitboxComponent(Vector2D<double> size, std::size_t health) : _size(size), _health(health), _maxHealth(health), _touch(false)
{
}

Vector2D<double> LifeHitboxComponent::getSize() const
{
    return (_size);
}

void LifeHitboxComponent::heal(std::size_t health)
{
    if (_health + health > _maxHealth)
        _health = _maxHealth;
    else
        _health += health;
}

void LifeHitboxComponent::takeDamages(std::size_t damages)
{
    if (_health < damages)
        _health = 0;
    else
        _health -= damages;
}

bool LifeHitboxComponent::isAlive()
{
    return (_health > 0);
}

void LifeHitboxComponent::setTouch(bool status)
{
    _touch = status;
}

bool LifeHitboxComponent::hasTouch() const
{
    return (_touch);
}

void destroyLifeHitbox(Registry &registry, Entity const &entity)
{
    SparseArray<LifeHitboxComponent> &healthArray = registry.getComponents<LifeHitboxComponent>();

    if (healthArray.size() > entity)
        healthArray[entity].reset();
}
