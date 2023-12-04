/*
** EPITECH PROJECT, 2023
** GameEngine
** File description:
** MobComponent
*/

#pragma once

#include "IComponent.hpp"

class BossComponent : public IComponent{
    public:
        BossComponent();
        size_t _phase = 0;
        int _nbrShot = 0;
        size_t _targetPlayer;
        double _direction = 1;
        double _moveFrame = 0;
        int _actionType = 0;

    private:
};

void destroyBoss(Registry &registry, Entity const &entity);
