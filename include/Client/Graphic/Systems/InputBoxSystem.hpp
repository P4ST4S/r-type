/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** InputBoxSystem
*/

#pragma once

#include "ASystem.hpp"
#include "IGraphic.hpp"
#include "PositionComponent.hpp"
#include "InputBoxComponent.hpp"

class InputBoxSystem : public ASystem {
    public:
        InputBoxSystem(std::shared_ptr<RGraphic::IGraphic> lib);
        ~InputBoxSystem();

        void operator()(Registry &registry, SparseArray<PositionComponent> &positionArray, SparseArray<InputBoxComponent> &inputBoxArray);

    protected:
    private:
        std::shared_ptr<RGraphic::IGraphic> _lib;
};
