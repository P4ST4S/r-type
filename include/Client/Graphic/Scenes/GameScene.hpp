/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** GameScene
*/

#pragma once

#include "TextureBank.hpp"
#include "ButtonComponent.hpp"
#include "TextComponent.hpp"
#include "PositionComponent.hpp"
#include "TextureComponent.hpp"
#include "IScene.hpp"
#include "HandleEntity.hpp"
#include "Protocol.hpp"
#include "SceneManagerSystem.hpp"
#include "InputEventListener.hpp"
#include "GlobalSettings.hpp"

#include <iostream>

class GameScene : public IScene {
    public:
        void loadScene(SceneManagerSystem &manager);
        void run(SceneManagerSystem &manager);
        void unloadScene(SceneManagerSystem &manager);

        void handleInput(SceneManagerSystem &manager);
    protected:
        int _backButton;
        int _quitButton;
        int _endButton;
        RGraphic::RInputs _lastInput;
        bool _paused = false;
};
