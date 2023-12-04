/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** SpaceshipsUpdatingSystem
*/

#pragma once

#include "ASystem.hpp"
#include "HealthComponent.hpp"

class HealthUpdatingSystem : public ASystem {
    public:
        void operator()(Registry &registry, SparseArray<HealthComponent> &HealthComponents);
};
