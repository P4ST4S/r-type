/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** InterpolationSystem
*/

#pragma once

#include "ASystem.hpp"
#include "InterpolationComponent.hpp"
#include "PositionComponent.hpp"
#include <iostream>

class InterpolationSystem : public ASystem {
    public:
        InterpolationSystem(double &screenWith, double &screenHeight);
        ~InterpolationSystem();

        void operator()(Registry &registry,
                SparseArray<InterpolationComponent> &interpolationComponents,
                SparseArray<PositionComponent> &positionComponents);

    protected:
        double &_screenWidth;
        double &_screenHeight;
};
