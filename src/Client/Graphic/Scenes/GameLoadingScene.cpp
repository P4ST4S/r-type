/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** GameLoadingScene
*/

#include "GameLoadingScene.hpp"
#include "GlobalSettings.hpp"
#include "SceneManagerSystem.hpp"

void GameLoadingScene::loadScene(SceneManagerSystem &manager)
{
    Registry &registry = manager.getRegistry();
    Entity text = registry.spawnEntity();

    registry.addComponent<PositionComponent>(text, PositionComponent(20, GlobalSettings::_screenHeight - 70));
    registry.addComponent<TextComponent>(text, TextComponent("GAME LOADING...", 50, 3, {255, 255, 0, 255}, {0, 0, 0, 255}));

    this->_loaded = false;
}

void GameLoadingScene::run(SceneManagerSystem &manager)
{
    Registry &registry = manager.getRegistry();
    manager.getClientPkgHandler().receiveNewData();
    std::deque<Package> pkgs = manager.getClientPkgHandler().accessPackages();
    std::deque<Package> respPkgs = PackageParser::extractPackages(pkgs, {GAME_START});

    if (!this->_loaded) {
        // Loading client

        manager.getClientPkgHandler().sendBodylessPackage(CPN_TCP, GAME_START);
        this->_loaded = true;
    }

    if (!respPkgs.empty())
        manager.changeScene(registry, "Game");
}

void GameLoadingScene::unloadScene(SceneManagerSystem &manager)
{
    manager.getRegistry().killAllEntities();
}