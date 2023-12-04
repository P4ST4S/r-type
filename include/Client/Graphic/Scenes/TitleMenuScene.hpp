/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** TitleMenuScene
*/

#pragma once

#include "IScene.hpp"

#include "Scenes.hpp"
#include "GameEngine.hpp"
#include "GraphicEngine.hpp"
#include "GlobalSettings.hpp"

#include "TextureBank.hpp"
#include "ButtonComponent.hpp"
#include "TextComponent.hpp"
#include "PositionComponent.hpp"
#include "TextureComponent.hpp"
#include "InputEventListener.hpp"

class TitleMenuScene : public IScene {
    public:
        void loadScene(SceneManagerSystem &manager);
        void run(SceneManagerSystem &manager);
        void unloadScene(SceneManagerSystem &manager);
};

