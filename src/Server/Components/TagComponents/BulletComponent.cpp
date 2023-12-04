/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** BulletComponent
*/

#include "BulletComponent.hpp"

BulletComponent::BulletComponent()
{
}

void destroyBullet(Registry &registry, Entity const &entity)
{
    SparseArray<BulletComponent> &bulletArray = registry.getComponents<BulletComponent>();

    if (bulletArray.size() > entity)
        bulletArray[entity].reset();
}
