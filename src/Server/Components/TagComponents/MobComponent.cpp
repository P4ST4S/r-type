/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** MobComponent
*/

#include "MobComponent.hpp"
#include <iostream>

MobComponent::MobComponent()
{
}



void destroyMob(Registry &registry, Entity const &entity)
{
    SparseArray<MobComponent> &mobArray = registry.getComponents<MobComponent>();

    if (mobArray.size() > entity){
        mobArray[entity].reset();
    }
}
