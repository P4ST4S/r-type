/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** MissileSystem
*/

#include "MissileSystem.hpp"

void MissileSystem::operator()(Registry &registry, SparseArray<BulletComponent> &lifeArray, SparseArray<DamageHitboxComponent> &damageArray, SparseArray<InterpolationComponent> &identityArray, SparseArray<VelocityComponent> &velocityArray)
{
    Zipper<SparseArray<BulletComponent>, SparseArray<DamageHitboxComponent>, SparseArray<InterpolationComponent>, SparseArray<VelocityComponent>> missile(lifeArray, damageArray, identityArray, velocityArray);

    for (auto it = missile.begin(); it != missile.end(); it++) {
        InterpolationComponent &position = std::get<InterpolationComponent&>(*it);

        if (std::get<DamageHitboxComponent&>(*it).hasTouch() || std::get<InterpolationComponent&>(*it).getFinalPosition()._x > INTERNAL_WIDTH || position.getFinalPosition()._x < -500)
            KillEntityEvent::killEntity(registry, it.getIndex());
        else
            position.move(std::get<VelocityComponent&>(*it).getVelocity());
    }
}