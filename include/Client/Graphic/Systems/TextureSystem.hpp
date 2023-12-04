/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** TextureSystem
*/

#pragma once

#include "ASystem.hpp"
#include "IGraphic.hpp"
#include "PositionComponent.hpp"
#include "TextureComponent.hpp"

class TextureSystem : public ASystem {
    public:
        TextureSystem(std::shared_ptr<RGraphic::IGraphic> lib);
        ~TextureSystem();

        void operator()(Registry &registry, SparseArray<PositionComponent> &positionArray, SparseArray<TextureComponent> &TextureArray);
    
    protected:
    private:
        std::shared_ptr<RGraphic::IGraphic> _lib;
};
