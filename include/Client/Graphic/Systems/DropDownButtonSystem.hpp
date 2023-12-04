/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** DropDownButtonSystem
*/

#pragma once

#include "ASystem.hpp"
#include "IGraphic.hpp"
#include "PositionComponent.hpp"
#include "DropDownButtonComponent.hpp"

class DropDownButtonSystem : public ASystem {
    public:
        DropDownButtonSystem(std::shared_ptr<RGraphic::IGraphic> lib);
        ~DropDownButtonSystem();

        void operator()(Registry &registry, SparseArray<PositionComponent> &positionArray, SparseArray<DropDownButtonComponent> &dropDownButtonArray);

    protected:
    private:
        std::shared_ptr<RGraphic::IGraphic> _lib;
};
