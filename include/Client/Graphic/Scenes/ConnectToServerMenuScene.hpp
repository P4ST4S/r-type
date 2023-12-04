/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** ConnectToServerMenuScene
*/

#pragma once

#include "IScene.hpp"
#include "SceneManagerSystem.hpp"
#include "GlobalSettings.hpp"
#include "TextComponent.hpp"
#include "PositionComponent.hpp"
#include "InputBoxComponent.hpp"
#include "ButtonComponent.hpp"

#define ADDRESS_STR_SIZE 15
#define PORT_STR_SIZE 5

class ConnectToServerMenuScene : public IScene{
    public:

        void loadScene(SceneManagerSystem &manager);
        void run(SceneManagerSystem &manager);
        void unloadScene(SceneManagerSystem &manager);

    protected:
        std::string _address = "Address";
        std::string _port = "Port";
};
