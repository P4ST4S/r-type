/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** InterpolationMoveSystem
*/

#include "InterpolationMoveSystem.hpp"

InterpolationMoveSystem::InterpolationMoveSystem()
{
}

InterpolationMoveSystem::~InterpolationMoveSystem()
{
}

void InterpolationMoveSystem::operator()(Registry &registry,
                SparseArray<InterpolationComponent> &interpolationComponents)
{
    // std::cout << "INTERPOLATION MOVE SYSTEM" << std::endl;
    for (std::size_t i = 0; i < interpolationComponents.size(); ++i){
        if (interpolationComponents[i].has_value()) {
            interpolationComponents[i].value().applyMove();
        }
    }
}
