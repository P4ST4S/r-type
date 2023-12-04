/*
** EPITECH PROJECT, 2023
** DamageHitboxComponent.cpp
** File description:
** DamageHitboxComponent
*/

#include "DamageHitboxComponent.hpp"

DamageHitboxComponent::DamageHitboxComponent(double width, double height, std::size_t damages) : _size(width, height), _damages(damages), _touch(false)
{
}

DamageHitboxComponent::DamageHitboxComponent(Vector2D<double> size, std::size_t damages) : _size(size), _damages(damages), _touch(false)
{
}


Vector2D<double> DamageHitboxComponent::getSize() const
{
    return _size;
}

std::size_t DamageHitboxComponent::getDamages() const
{
    return (_damages);
}

void DamageHitboxComponent::setTouch(bool status)
{
    _touch = status;
}

bool DamageHitboxComponent::hasTouch() const
{
    return (_touch);
}

void destroyDamageHitbox(Registry &registry, Entity const &entity)
{
    SparseArray<DamageHitboxComponent> &damageArray = registry.getComponents<DamageHitboxComponent>();

    if (damageArray.size() > entity)
        damageArray[entity].reset();
}
