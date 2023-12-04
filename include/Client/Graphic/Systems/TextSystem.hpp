/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** TextSystem
*/

#pragma once

#include "ASystem.hpp"
#include "IGraphic.hpp"
#include "PositionComponent.hpp"
#include "TextComponent.hpp"

class TextSystem : public ASystem {
    public:
        TextSystem(std::shared_ptr<RGraphic::IGraphic> lib);
        ~TextSystem();

        void operator()(Registry &registry, SparseArray<PositionComponent> &positionArray, SparseArray<TextComponent> &TextArray);

    protected:
    private:
        std::shared_ptr<RGraphic::IGraphic> _lib;
};
