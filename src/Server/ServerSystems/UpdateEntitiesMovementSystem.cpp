/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** UpdateEntitiesEventSystem
*/

#include "UpdateEntitiesEventSystem.hpp"

UpdateEntitiesEventSystem::UpdateEntitiesEventSystem()
{
}

UpdateEntitiesEventSystem::~UpdateEntitiesEventSystem()
{
}

void UpdateEntitiesEventSystem::operator()(Registry &registry, SparseArray<InterpolationComponent> &interpolationArray, SparseArray<PlayerEventsComponent> &eventArray, SparseArray<WeaponComponent> &weaponArray, SparseArray<VelocityComponent> &velocityArray)
{
    // std::cout << "UPDATE ENTITIES MOVEMENT SYSTEM" << std::endl;
    Zipper<SparseArray<InterpolationComponent>, SparseArray<PlayerEventsComponent>, SparseArray<WeaponComponent>, SparseArray<VelocityComponent>> zipper(interpolationArray, eventArray, weaponArray, velocityArray);

    for (auto it = zipper.begin(); it != zipper.end(); it++){
        InterpolationComponent &movement = std::get<InterpolationComponent&>(*it);
        PlayerEventsComponent &playerEvents = std::get<PlayerEventsComponent&>(*it);
        WeaponComponent &weapon = std::get<WeaponComponent&>(*it);
        VelocityComponent &velocity = std::get<VelocityComponent&>(*it);

        movement.applyMove();
        movement.move(Vector2D<double>(
            (playerEvents.getEvents()._right - playerEvents.getEvents()._left) * velocity.getVelocity()._x,
            (playerEvents.getEvents()._down - playerEvents.getEvents()._up) * velocity.getVelocity()._y
        ));
        movement.replacePosition();
        weapon.updateCooldown();
        if (playerEvents.getEvents()._shoot)
            weapon.shoot(registry, movement.getFinalPosition(), 4);
    }
}
