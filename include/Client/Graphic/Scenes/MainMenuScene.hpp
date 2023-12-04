/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** MainMenuScene
*/



#pragma once

#include "IScene.hpp"
#include "SceneManagerSystem.hpp"
#include "GlobalSettings.hpp"
#include "TextComponent.hpp"
#include "PositionComponent.hpp"
#include "ButtonComponent.hpp"

class MainMenuScene : public IScene {
    public:
        void loadScene(SceneManagerSystem &manager);
        void run(SceneManagerSystem &manager);
        void unloadScene(SceneManagerSystem &manager);
};
