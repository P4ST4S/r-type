/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** ConnectingToServerMenuScene
*/

#pragma once

#include "IScene.hpp"
#include "GlobalSettings.hpp"
#include "TextComponent.hpp"
#include "PositionComponent.hpp"

class ConnectingToServerMenuScene : public IScene{
    public:
        void loadScene(SceneManagerSystem &manager);
        void run(SceneManagerSystem &manager);
        void unloadScene(SceneManagerSystem &manager);
    protected:
        std::time_t _time;
};
