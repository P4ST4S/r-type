/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** UpdateEntitiesEventSystem
*/

#pragma once

#include "Registry.hpp"
#include "GameEngine.hpp"
#include "WeaponComponent.hpp"

#include <iostream>

class UpdateEntitiesEventSystem {
public:
    UpdateEntitiesEventSystem();
    ~UpdateEntitiesEventSystem();

    void operator()(Registry &registry, SparseArray<InterpolationComponent> &interpolationArray, SparseArray<PlayerEventsComponent> &eventArray, SparseArray<WeaponComponent> &weaponArray, SparseArray<VelocityComponent> &velocityArray);
};
