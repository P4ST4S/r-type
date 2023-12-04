/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** BossComponent
*/

#include "BossComponent.hpp"
#include <iostream>

BossComponent::BossComponent()
{
}



void destroyBoss(Registry &registry, Entity const &entity)
{
    SparseArray<BossComponent> &mobArray = registry.getComponents<BossComponent>();

    if (mobArray.size() > entity){
        mobArray[entity].reset();
    }
}
