/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** ServerLobbyMenuScene
*/

#pragma once

#include "IScene.hpp"
#include "TextComponent.hpp"
#include "PositionComponent.hpp"

class ServerLobbyMenuScene : public IScene {
    public:
        void loadScene(SceneManagerSystem &manager);
        void run(SceneManagerSystem &manager);
        void unloadScene(SceneManagerSystem &manager);
};
