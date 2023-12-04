/*
** EPITECH PROJECT, 2023
** GameEngine
** File description:
** PlayerComponent
*/

#pragma once

#include "IComponent.hpp"

class PlayerComponent : public IComponent{
    public:
        PlayerComponent();
};

void destroyPlayer(Registry &registry, Entity const &entity);
