/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** SettingsMenuScene
*/

#pragma once

#include "IScene.hpp"
#include "SceneManagerSystem.hpp"
#include "GlobalSettings.hpp"
#include "PositionComponent.hpp"
#include "TextComponent.hpp"
#include "CheckboxComponent.hpp"
#include "ButtonComponent.hpp"
#include "DropDownButtonComponent.hpp"
#include "SliderComponent.hpp"

class SettingsMenuScene : public IScene {
    public:
        void loadScene(SceneManagerSystem &manager);
        void run(SceneManagerSystem &manager);
        void unloadScene(SceneManagerSystem &manager);
};
