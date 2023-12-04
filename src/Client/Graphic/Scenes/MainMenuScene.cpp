/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** TitleMenuScene
*/

#include "MainMenuScene.hpp"

using namespace GlobalSettings;

void MainMenuScene::loadScene(SceneManagerSystem &manager)
{
    Registry &registry = manager.getRegistry();
    RGraphic::IGraphic &lib = manager.getGraphic();
    manager.getClientPkgHandler().disconnect();

    Entity img = registry.spawnEntity();
    Entity button = registry.spawnEntity();
    double buttonWidth = _screenWidth * 0.2;
    double buttonHeight = _screenHeight * 0.1;
    double buttonSpacing = buttonHeight * 1.5;
    RGraphic::RVect2_t buttonPos = { static_cast<float>((_screenWidth - buttonWidth) / 2), static_cast<float>((_screenHeight - buttonHeight) / 2) };

    registry.addComponent<PositionComponent>(img, PositionComponent(_screenWidth / 2, _screenHeight / 2));
    TextureComponent &texture = *registry.addComponent<TextureComponent>(img, TextureComponent());
    TextureInfo sprite = TexturesGlobal::BANK[CUPHEAD_BACKGROUND];
    texture.addTexture(CUPHEAD_BACKGROUND, lib.loadTexture(sprite.path), sprite.dimension, sprite.nframes, static_cast<double>(_screenWidth) / sprite.dimension.x, 15);

    registry.addComponent<PositionComponent>(button, PositionComponent(buttonPos.x, buttonPos.y));
    registry.addComponent<ButtonComponent>(button, ButtonComponent(Vector2D<double>(buttonWidth, buttonHeight), "Play", true, [&]() {
        manager.changeScene(registry, "ConnectToServerMenu");
    }));

    button = registry.spawnEntity();
    registry.addComponent<PositionComponent>(button, PositionComponent(buttonPos.x, buttonPos.y + buttonSpacing));
    registry.addComponent<ButtonComponent>(button, ButtonComponent(Vector2D<double>(buttonWidth, buttonHeight), "Settings", true, [&]() {
        manager.changeScene(registry, "SettingsMenu");
    }));

    button = registry.spawnEntity();
    registry.addComponent<PositionComponent>(button, PositionComponent(buttonPos.x, buttonPos.y + buttonSpacing * 2));
    registry.addComponent<ButtonComponent>(button, ButtonComponent(Vector2D<double>(buttonWidth, buttonHeight), "Quit", true, [&]() {
        registry.setLoop(false);
    }));
}

void MainMenuScene::run(SceneManagerSystem &manager)
{
}

void MainMenuScene::unloadScene(SceneManagerSystem &manager)
{
    manager.getRegistry().killAllEntities();
}
