/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** ButtonSystem
*/

#pragma once

#include "ASystem.hpp"
#include "IGraphic.hpp"
#include "PositionComponent.hpp"
#include "ButtonComponent.hpp"

class ButtonSystem : public ASystem {
    public:
        ButtonSystem(std::shared_ptr<RGraphic::IGraphic> lib);
        ~ButtonSystem();

        void operator()(Registry &registry, SparseArray<PositionComponent> &positionArray, SparseArray<ButtonComponent> &buttonArray);

    protected:
    private:
        std::shared_ptr<RGraphic::IGraphic> _lib;
};
