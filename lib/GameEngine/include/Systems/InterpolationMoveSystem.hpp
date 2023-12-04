/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** InterpolationMoveSystem
*/

#pragma once

#include "ASystem.hpp"
#include "InterpolationComponent.hpp"

#include <iostream>

class InterpolationMoveSystem : public ASystem {
public:
    InterpolationMoveSystem();
    ~InterpolationMoveSystem();

    void operator()(Registry &registry,
        SparseArray<InterpolationComponent> &interpolationComponents);
};
