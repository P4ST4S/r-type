/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** HitboxSystem
*/

#pragma once

#include "ASystem.hpp"
#include "LifeHitboxComponent.hpp"
#include "DamageHitboxComponent.hpp"
#include "IdentityComponent.hpp"
#include "InterpolationComponent.hpp"
#include "EntityEnums.hpp"
#include "KillEntityEvent.hpp"
#include "Collision.hpp"

class HitboxSystem : public ASystem {
    public:
        void operator()(Registry &registry, SparseArray<LifeHitboxComponent> &lifeArray, SparseArray<DamageHitboxComponent> &damageArray, SparseArray<IdentityComponent> &identityArray, SparseArray<InterpolationComponent> &positionArray);
};
