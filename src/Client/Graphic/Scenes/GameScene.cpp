/*
** EPITECH PROJECT, 2023
** HomeScene
** File description:
** HomeScene
*/

#include "GameScene.hpp"

using namespace GlobalSettings;

void GameScene::loadScene(SceneManagerSystem &manager)
{
    RGraphic::IGraphic &lib = manager.getGraphic();
    Registry &registry = manager.getRegistry();
    RGraphic::RMusic &music = manager.getMusicTheme();
    Entity button = registry.spawnEntity();
    Entity quit = registry.spawnEntity();
    Entity back = registry.spawnEntity();
    Entity end = registry.spawnEntity();

    registry.addComponent<PositionComponent>(quit, PositionComponent(_screenWidth * 0.4, _screenHeight * 0.4));
    registry.addComponent<ButtonComponent>(quit, ButtonComponent(Vector2D<double>(_screenWidth * 0.2, _screenHeight * 0.1), "Back", false, [&]() {
        manager.changeScene(registry, "MainMenu");
    }));
    registry.addComponent<PositionComponent>(back, PositionComponent(_screenWidth * 0.4, _screenHeight * 0.6));
    registry.addComponent<ButtonComponent>(back, ButtonComponent(Vector2D<double>(_screenWidth * 0.2, _screenHeight * 0.1), "Quit", false, [&]() {
        registry.setLoop(false);
    }));

    registry.addComponent<PositionComponent>(end, PositionComponent(_screenWidth * 0.4, _screenHeight * 0.4));
    registry.addComponent<ButtonComponent>(end, ButtonComponent(Vector2D<double>(_screenWidth * 0.2, _screenHeight * 0.1), "Back", false, [&]() {
        manager.changeScene(registry, "MainMenu");
    }));
    _quitButton = quit;
    _backButton = back;
    _endButton = end;

    button = registry.spawnEntity();
    registry.addComponent<PositionComponent>(button, PositionComponent(_screenWidth / 2, _screenHeight / 2));
    TextureComponent &texture = *registry.addComponent<TextureComponent>(button, TextureComponent());
    TextureInfo sprite = TexturesGlobal::BANK[BIRDHOUSE_BACKGROUND];
    texture.addTexture(BIRDHOUSE_BACKGROUND, lib.loadTexture(sprite.path), sprite.dimension, sprite.nframes, 7569 / _screenWidth);

    music = lib.loadMusic("assets/music/cuphead_fight_theme.ogg");
    lib.playMusic(music);
    manager.getEntityHandler().resetFirstMovePkg();

    manager.getClientPkgHandler().sendResponse(CPN_TCP, PLAYER_LIST, RESP_INFORMATION);
}

void GameScene::run(SceneManagerSystem &manager) {
    Registry &registry = manager.getRegistry();
    RGraphic::IGraphic &lib = manager.getGraphic();
    HandleEntity &handleEntity = manager.getEntityHandler();
    SparseArray<ButtonComponent> &buttons = registry.getComponents<ButtonComponent>();

    if (!manager.getClientPkgHandler().isConnected()) {
        manager.changeScene(registry, "MainMenu");
        return;
    }

    lib.updateMusic(manager.getMusicTheme());
    manager.getClientPkgHandler().receiveNewData();

    std::deque<Package> &pkgs = manager.getClientPkgHandler().accessPackages();

    std::deque<Package> playerListPkgs = PackageParser::extractPackages(pkgs, {PLAYER_LIST});
    std::deque<Package> actionPkgs = PackageParser::extractPackages(pkgs, {
        NEW_ENTITIES,
        DESTROY_ENTITIES,
        MOVE_ENTITIES
    });

    if (!playerListPkgs.empty()) {
        int count = playerListPkgs.back().getBody<DynamicBody<PlayerList_t>>().elemCount();
    }
    // Entity health1 = registry.spawnEntity();
    // Entity health2 = registry.spawnEntity();
    // Entity health3 = registry.spawnEntity();
    // Entity health4 = registry.spawnEntity();
    // registry.addComponent<PosisionComponent>(health1, PosisionComponent(_screenWidth * 0.05, _screenHeight * 0.05));
    // registry.addComponent<PosisionComponent>(health2, PosisionComponent(_screenWidth * 0.05 + _screenWidth * 0.9 / 4, _screenHeight * 0.05));
    // registry.addComponent<PosisionComponent>(health3, PosisionComponent(_screenWidth * 0.05 + (_screenWidth * 0.9 / 4) * 2, _screenHeight * 0.05));
    // registry.addComponent<PosisionComponent>(health4, PosisionComponent(_screenWidth * 0.05 + (_screenWidth * 0.9 / 4) * 3, _screenHeight * 0.05));
    // gethp

    handleEntity.packetsInterpreter(actionPkgs);
    handleEntity.UpdatedyingEntities();
    this->handleInput(manager);

    std::deque<Package> endPkgs = PackageParser::extractPackages(pkgs, {
        END_GAME
    });
    if (!endPkgs.empty()) {
        Entity text = registry.spawnEntity();
        std::string str = ((endPkgs.back().getBody<Body<EndGame_t>>().get()._win) ? "VICTORY" : "LOSE");
        ButtonComponent& endButton = *buttons[registry.entityFromIndex(_endButton)];
        endButton.setShown(true);
        registry.addComponent<PositionComponent>(text, PositionComponent(20, _screenHeight - 70));
        registry.addComponent<TextComponent>(text, TextComponent(str, 50, 3, {255, 255, 255, 255}, {0, 0, 0, 255}));
    }
}

void GameScene::unloadScene(SceneManagerSystem &manager)
{
    RGraphic::IGraphic &lib = manager.getGraphic();

    manager.getRegistry().killAllEntities();
    lib.unloadMusic(manager.getMusicTheme());
}

void GameScene::handleInput(SceneManagerSystem &manager)
{
    RGraphic::RInputs currentInput = {0};
    Registry &registry = manager.getRegistry();
    std::deque<InputEvent> inputEvents = registry.getEvent<InputEvent>();
    SparseArray<ButtonComponent> &buttons = registry.getComponents<ButtonComponent>();

    if (!inputEvents.empty())
        currentInput = inputEvents.front().getInputs();
    registry.clearEvent<InputEvent>();

    if (currentInput.pause) {
        ButtonComponent& quitButton = *buttons[registry.entityFromIndex(_quitButton)];
        ButtonComponent& backButton = *buttons[registry.entityFromIndex(_backButton)];
        quitButton.setShown(!quitButton.getShown());
        backButton.setShown(!backButton.getShown());
    }
    if (memcmp(&currentInput, &(this->_lastInput), sizeof(currentInput)) != 0){
        Package inputPkg = manager.getClientPkgHandler().createPackage(CLIENT_MOVE_EVENT);
        Body<ClientMoveEvent_t> &body = inputPkg.getBody<Body<ClientMoveEvent_t>>();
        Package shootPkg = manager.getClientPkgHandler().createPackage(CLIENT_SHOOT_EVENT);
        Body<ClientShootEvent_t> &shootBody = shootPkg.getBody<Body<ClientShootEvent_t>>();

        body.get()._down = currentInput.down;
        body.get()._up = currentInput.up;
        body.get()._left = currentInput.left;
        body.get()._right = currentInput.right;

        manager.getClientPkgHandler().sendPackage(CPN_UDP, inputPkg);

        shootBody.get()._shoot = currentInput.shoot;
        manager.getClientPkgHandler().sendPackage(CPN_UDP, shootPkg);
        this->_lastInput = currentInput;
    }
}
