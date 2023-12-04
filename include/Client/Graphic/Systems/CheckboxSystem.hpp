/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** CheckboxSystem
*/

#pragma once

#include "ASystem.hpp"
#include "IGraphic.hpp"
#include "PositionComponent.hpp"
#include "CheckboxComponent.hpp"

class CheckboxSystem : public ASystem {
    public:
        CheckboxSystem(std::shared_ptr<RGraphic::IGraphic> lib);
        ~CheckboxSystem();

        void operator()(Registry &registry, SparseArray<PositionComponent> &positionArray, SparseArray<CheckboxComponent> &CheckboxArray);

    protected:
    private:
        std::shared_ptr<RGraphic::IGraphic> _lib;
};
