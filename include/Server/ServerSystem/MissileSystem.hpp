/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** MissileSystem
*/

#pragma once

#include "ASystem.hpp"
#include "DamageHitboxComponent.hpp"
#include "InterpolationComponent.hpp"
#include "EntityEnums.hpp"
#include "BulletComponent.hpp"
#include "KillEntityEvent.hpp"
#include "Collision.hpp"

class MissileSystem : public ASystem {
    public:
        void operator()(Registry &registry, SparseArray<BulletComponent> &lifeArray, SparseArray<DamageHitboxComponent> &damageArray, SparseArray<InterpolationComponent> &identityArray, SparseArray<VelocityComponent> &velocityArray);
};
