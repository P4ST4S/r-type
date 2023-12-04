/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** SliderSystem
*/

#pragma once

#include "ASystem.hpp"
#include "IGraphic.hpp"
#include "PositionComponent.hpp"
#include "SliderComponent.hpp"

class SliderSystem : public ASystem {
    public:
        SliderSystem(std::shared_ptr<RGraphic::IGraphic> lib);
        ~SliderSystem();

        void operator()(Registry &registry, SparseArray<PositionComponent> &positionArray, SparseArray<SliderComponent> &SliderArray);

    protected:
    private:
        std::shared_ptr<RGraphic::IGraphic> _lib;
};
