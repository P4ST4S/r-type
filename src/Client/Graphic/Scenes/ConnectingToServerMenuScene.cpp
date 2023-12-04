/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** ConnectingToServerMenuScene
*/

#include "Scenes.hpp"
#include "GameEngine.hpp"
#include "GraphicEngine.hpp"
#include "GlobalSettings.hpp"

#include "TextureBank.hpp"
#include "ButtonComponent.hpp"
#include "TextComponent.hpp"
#include "PositionComponent.hpp"
#include "TextureComponent.hpp"

using namespace GlobalSettings;

void ConnectingToServerMenuScene::loadScene(SceneManagerSystem &manager)
{
    Registry &registry = manager.getRegistry();
    Entity text = registry.spawnEntity();
    Entity button = registry.spawnEntity();
    double buttonWidth = _screenWidth * 0.2;
    double buttonHeight = _screenHeight * 0.1;

    registry.addComponent<PositionComponent>(text, PositionComponent(_screenWidth * 0.05, _screenHeight * 0.9));
    registry.addComponent<TextComponent>(text, TextComponent("CONNECTING TO SERVER...", 50, 3, {255, 255, 255, 255}, {0, 0, 0, 255}));

    registry.addComponent<PositionComponent>(button, PositionComponent(20, 20));
    registry.addComponent<ButtonComponent>(button, ButtonComponent(Vector2D<double>(buttonWidth, buttonHeight), "Back", true, [&]() {
        manager.changeScene(registry, "ConnectToServerMenu");
    }));

    this->_time = std::time(nullptr);
}

void ConnectingToServerMenuScene::run(SceneManagerSystem &manager)
{
    Registry &registry = manager.getRegistry();
    manager.getClientPkgHandler().receiveNewData();
    Package pkg;
    std::deque<Package> pkgs = manager.getClientPkgHandler().getPackages();
    std::deque<Package> respPkgs = PackageParser::extractPackages(pkgs, {RESPONSE});

    while (!respPkgs.empty()) {
        pkg = respPkgs.front();
        respPkgs.pop_front();
        Body<Response_t> &body = pkg.getBody<Body<Response_t>>();
        if (body.get()._commandType == CLIENT_CONNECTION_REQUEST) {
            switch (body.get()._responseStatus) {
            case RESP_OK:
                manager.changeScene(registry, "ServerLobbyMenu");
                break;
            }
        } else {
            pkgs.push_back(pkg);
        }
    }
    if (this->_time + 4 < std::time(nullptr) || !(manager.getClientPkgHandler().isConnected())) {
        manager.changeScene(registry, "ConnectToServerMenu");
    }
}

void ConnectingToServerMenuScene::unloadScene(SceneManagerSystem &manager)
{
    manager.getRegistry().killAllEntities();
}
