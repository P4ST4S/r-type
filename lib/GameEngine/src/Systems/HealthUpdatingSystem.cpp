/*
** EPITECH PROJECT, 2023
** GameEngine
** File description:
** HealthUpdatingSystem
*/

#include "HealthUpdatingSystem.hpp"

void HealthUpdatingSystem::operator()(Registry &registry, SparseArray<HealthComponent> &healthComponent)
{
    Zipper<SparseArray<HealthComponent>> zipper(healthComponent);

    for (auto it = zipper.begin(); it != zipper.end(); it++){
        HealthComponent &health = std::get<HealthComponent&>(*it);       
    }
}
