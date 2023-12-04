/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** ServerLobbyMenuScene
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

void ServerLobbyMenuScene::loadScene(SceneManagerSystem &manager)
{
    Registry &registry = manager.getRegistry();

    Entity text = registry.spawnEntity();
    Entity button = registry.spawnEntity();
    double buttonWidth = _screenWidth * 0.2;
    double buttonHeight = _screenHeight * 0.1;
    double buttonSpacing = buttonHeight * 1.5;
    double buttonSpacingW = buttonWidth * 0.2;
    RGraphic::RVect2_t buttonPos = { static_cast<float>((_screenWidth - buttonWidth) / 2), static_cast<float>((_screenHeight - buttonHeight) / 2) };

    registry.addComponent<PositionComponent>(button, PositionComponent(_screenWidth * 0.05, _screenHeight * 0.05));
    registry.addComponent<ButtonComponent>(button, ButtonComponent(Vector2D<double>(buttonWidth, buttonHeight), "Back", true, [&]() {
        manager.changeScene(registry, "ConnectToServerMenu");
    }));

    registry.addComponent<PositionComponent>(text, PositionComponent(_screenWidth * 0.05, _screenHeight * 0.9));
    registry.addComponent<TextComponent>(text, TextComponent("CONNECTED TO SERVER!", 50, 3, {255, 255, 0, 255}, {0, 0, 0, 255}));

    button = registry.spawnEntity();
    registry.addComponent<PositionComponent>(button, PositionComponent(_screenWidth * 0.95 - buttonWidth, _screenHeight * 0.95 - buttonHeight));
    registry.addComponent<ButtonComponent>(button, ButtonComponent(Vector2D<double>(buttonWidth, buttonHeight), "Run Game", true, [&]() {
        manager.getClientPkgHandler().sendBodylessPackage(CPN_TCP, START_GAME_LOADING);
    }));

}

void ServerLobbyMenuScene::run(SceneManagerSystem &manager)
{
    Registry &registry = manager.getRegistry();
    manager.getClientPkgHandler().receiveNewData();
    std::deque<Package> pkgs = manager.getClientPkgHandler().accessPackages();
    std::deque<Package> respPkgs = PackageParser::extractPackages(pkgs, {START_GAME_LOADING});

    if (!respPkgs.empty())
        manager.changeScene(registry, "GameLoading");
}

void ServerLobbyMenuScene::unloadScene(SceneManagerSystem &manager)
{
    manager.getRegistry().killAllEntities();
}
