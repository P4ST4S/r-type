/*
** EPITECH PROJECT, 2023
** Hitbox
** File description:
** HitboxSystem
*/

#include "HitboxSystem.hpp"

void HitboxSystem::operator()(Registry &registry, SparseArray<LifeHitboxComponent> &lifeArray, SparseArray<DamageHitboxComponent> &damageArray, SparseArray<IdentityComponent> &identityArray, SparseArray<InterpolationComponent> &positionArray)
{
    Zipper<SparseArray<DamageHitboxComponent>, SparseArray<IdentityComponent>, SparseArray<InterpolationComponent>> damageZipper(damageArray, identityArray, positionArray);

    for (auto damageIt = damageZipper.begin(); damageIt != damageZipper.end(); damageIt++){
        Zipper<SparseArray<LifeHitboxComponent>, SparseArray<IdentityComponent>, SparseArray<InterpolationComponent>> lifeZipper(lifeArray, identityArray, positionArray);

        for (auto lifeIt = lifeZipper.begin(); lifeIt != lifeZipper.end(); lifeIt++) {
            int lifeId = std::get<IdentityComponent&>(*lifeIt).getIdentity();
            int damageId = std::get<IdentityComponent&>(*damageIt).getIdentity();

            if ((lifeId & 0xF) == (damageId & 0xF) ||
                ((lifeId & 0xF) <= PLAYER3_ID && (damageId & 0xF) <= PLAYER3_ID) ||
                (((lifeId >> 4) == MOB_ID || (lifeId >> 4) == GHOST_BOSS_ID) && ((damageId >> 4) == MOB_ID || (damageId >> 4) == GHOST_BOSS_ID)))
                continue;

            LifeHitboxComponent &lifeHitbox = std::get<LifeHitboxComponent&>(*lifeIt);
            DamageHitboxComponent &damageHitbox = std::get<DamageHitboxComponent&>(*damageIt);
            InterpolationComponent &lifePosition = std::get<InterpolationComponent&>(*lifeIt);
            InterpolationComponent &damagePosition = std::get<InterpolationComponent&>(*damageIt);

            if (Geometry::rectanglesOverlap({lifePosition.getFinalPosition(), lifeHitbox.getSize()},
                                            {damagePosition.getFinalPosition(), damageHitbox.getSize()})) {
                lifeHitbox.takeDamages(damageHitbox.getDamages());
                damageHitbox.setTouch(true);
                if (!lifeHitbox.isAlive())
                    KillEntityEvent::killEntity(registry, lifeIt.getIndex());
            }

        }
    }
}
