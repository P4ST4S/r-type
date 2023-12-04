/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** PlayerComponent
*/

#include "PlayerComponent.hpp"

PlayerComponent::PlayerComponent()
{
}


void destroyPlayer(Registry &registry, Entity const &entity)
{
    SparseArray<PlayerComponent> &playerArray = registry.getComponents<PlayerComponent>();

    if (playerArray.size() > entity)
        playerArray[entity].reset();
}
