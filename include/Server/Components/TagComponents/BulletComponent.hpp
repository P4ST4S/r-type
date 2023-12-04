/*
** EPITECH PROJECT, 2023
** GameEngine
** File description:
** BulletComponent
*/

#pragma once

#include "IComponent.hpp"

class BulletComponent : public IComponent{
    public:
        BulletComponent();
};

void destroyBullet(Registry &registry, Entity const &entity);
