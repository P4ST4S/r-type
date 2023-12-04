/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** TitleMenuScene
*/

#include "TitleMenuScene.hpp"

using namespace GlobalSettings;

void TitleMenuScene::loadScene(SceneManagerSystem &manager)
{
    Registry &registry = manager.getRegistry();
    RGraphic::IGraphic &lib = manager.getGraphic();
    RGraphic::RMusic &music = manager.getMusicTheme();
    TextureInfo sprite = TexturesGlobal::BANK[CUPHEAD_BACKGROUND];
    TextureInfo sprite2 = TexturesGlobal::BANK[TITLE_BROTHERS];

    manager.getClientPkgHandler().disconnect();

    Entity img = registry.spawnEntity();
    Entity characters = registry.spawnEntity();
    Entity text = registry.spawnEntity();
    Entity generic = registry.spawnEntity();

    TextureComponent &texture = *registry.addComponent<TextureComponent>(img, TextureComponent());
    registry.addComponent<PositionComponent>(img, PositionComponent(_screenWidth / 2, _screenHeight / 2));
    texture.addTexture(CUPHEAD_BACKGROUND, lib.loadTexture(sprite.path), sprite.dimension, sprite.nframes, _screenWidth / sprite.dimension.x);

    TextureComponent &texture2 = *registry.addComponent<TextureComponent>(characters, TextureComponent());
    double scale = (_screenHeight / 1.5) / sprite2.dimension.y;
    registry.addComponent<PositionComponent>(characters, PositionComponent(_screenWidth / 2, _screenHeight - sprite2.dimension.y / 2 * scale));
    texture2.addTexture(TITLE_BROTHERS, lib.loadTexture(sprite2.path), sprite2.dimension, sprite2.nframes, scale, 24);

    uint16_t fontSize = _screenHeight * 0.1;
    registry.addComponent<PositionComponent>(text, PositionComponent(_screenWidth - 20 * 10 * fontSize / 32, _screenHeight * 0.9));
    registry.addComponent<TextComponent>(text, TextComponent("PRESS SPACE TO PLAY", fontSize, 3, {255, 255, 0, 255}, {0, 0, 0, 255}));

    music = lib.loadMusic("assets/music/cuphead_opening_theme.ogg");
    lib.playMusic(music);
}

void TitleMenuScene::run(SceneManagerSystem &manager)
{
    Registry &registry = manager.getRegistry();
    RGraphic::IGraphic &lib = manager.getGraphic();
    std::deque<InputEvent> inputsEvent = registry.getEvent<InputEvent>();

    lib.updateMusic(manager.getMusicTheme());
    if (inputsEvent.empty())
        return;

    if (inputsEvent.front().getInputs().shoot) {
        manager.changeScene(registry, "MainMenu");
    }
}

void TitleMenuScene::unloadScene(SceneManagerSystem &manager)
{
    RGraphic::IGraphic &lib = manager.getGraphic();
    Registry &registry = manager.getRegistry();

    lib.unloadMusic(manager.getMusicTheme());
    registry.killAllEntities();
}
