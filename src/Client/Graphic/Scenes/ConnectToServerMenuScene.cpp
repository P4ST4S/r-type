/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** TitleMenuScene
*/

#include "ConnectToServerMenuScene.hpp"

using namespace GlobalSettings;

void ConnectToServerMenuScene::loadScene(SceneManagerSystem &manager)
{
    RGraphic::IGraphic &lib = manager.getGraphic();
    Registry &registry = manager.getRegistry();

    manager.getClientPkgHandler().disconnect();

    Entity img = registry.spawnEntity();
    Entity text = registry.spawnEntity();
    Entity button = registry.spawnEntity();
    Entity addressInput = registry.spawnEntity();
    Entity portInput = registry.spawnEntity();
    double buttonWidth = _screenWidth * 0.2;
    double buttonHeight = _screenHeight * 0.1;
    double buttonSpacing = buttonHeight * 1.5;
    double buttonSpacingW = buttonWidth * 0.2;
    RGraphic::RVect2_t buttonPos = { static_cast<float>((_screenWidth - buttonWidth) / 2), static_cast<float>((_screenHeight - buttonHeight) / 2) };

    registry.addComponent<PositionComponent>(img, PositionComponent(0, 0));
    TextureComponent &texture = *registry.addComponent<TextureComponent>(img, TextureComponent());

    registry.addComponent<PositionComponent>(addressInput, PositionComponent((_screenWidth - buttonWidth) / 2, _screenHeight / 2 - buttonHeight));
    InputBoxComponent &addressBox = *registry.addComponent<InputBoxComponent>(addressInput, InputBoxComponent(Vector2D<double>(buttonWidth, buttonHeight), this->_address.c_str(), ADDRESS_STR_SIZE));

    registry.addComponent<PositionComponent>(portInput, PositionComponent((_screenWidth - buttonWidth) / 2, _screenHeight / 2 - buttonHeight + buttonSpacing));
    InputBoxComponent &portBox = *registry.addComponent<InputBoxComponent>(portInput, InputBoxComponent(Vector2D<double>(buttonWidth, buttonHeight), this->_port.c_str(), PORT_STR_SIZE));


    registry.addComponent<PositionComponent>(button, PositionComponent(_screenWidth * 0.05, _screenHeight * 0.05));
    registry.addComponent<ButtonComponent>(button, ButtonComponent(Vector2D<double>(buttonWidth, buttonHeight), "Back", true, [&]() {
        this->_port = portBox.getText();
        this->_address = addressBox.getText();
        manager.changeScene(registry, "MainMenu");
    }));

    button = registry.spawnEntity();
    registry.addComponent<PositionComponent>(button, PositionComponent(_screenWidth * 0.95 - buttonSpacingW - buttonWidth * 2, _screenHeight * 0.95 - buttonHeight));
    registry.addComponent<ButtonComponent>(button, ButtonComponent(Vector2D<double>(buttonWidth, buttonHeight), "Join Lobby", true, [&]() {
        this->_port = portBox.getText();
        this->_address = addressBox.getText();
        if (std::all_of(this->_port.begin(), this->_port.end(), ::isdigit) == false)
            return;
        if (manager.getClientPkgHandler().connectTo(this->_address, std::stoi(this->_port))) {
            Package pkg = manager.getClientPkgHandler().createPackage(CLIENT_CONNECTION_REQUEST);
            pkg.getBody<Body<ClientConnectionRequest_t>>().get()._clientType = CLI_TYPE_PLAYER;
            manager.getClientPkgHandler().sendPackage(CPN_TCP, pkg);
            manager.changeScene(registry, "ConnectingToServerMenu");
            return;
        }
        std::cout << "Failed to connect to server" << std::endl;
    }));

    button = registry.spawnEntity();
    registry.addComponent<PositionComponent>(button, PositionComponent(_screenWidth * 0.95 - buttonWidth, _screenHeight * 0.95 - buttonHeight));
    registry.addComponent<ButtonComponent>(button, ButtonComponent(Vector2D<double>(buttonWidth, buttonHeight), "Spectate Lobby", true, [&]() {
        this->_port = portBox.getText();
        this->_address = addressBox.getText();
        if (std::all_of(this->_port.begin(), this->_port.end(), ::isdigit) == false)
            return;
        if (manager.getClientPkgHandler().connectTo(this->_address, std::stoi(this->_port))) {
            Package pkg = manager.getClientPkgHandler().createPackage(CLIENT_CONNECTION_REQUEST);
            pkg.getBody<Body<ClientConnectionRequest_t>>().get()._clientType = CLI_TYPE_SPECTATOR;
            manager.getClientPkgHandler().sendPackage(CPN_TCP, pkg);
            manager.changeScene(registry, "ConnectingToServerMenu");
            return;
        }
        std::cout << "Failed to connect to server" << std::endl;
    }));
}

void ConnectToServerMenuScene::run(SceneManagerSystem &manager)
{
}

void ConnectToServerMenuScene::unloadScene(SceneManagerSystem &manager)
{
    manager.getRegistry().killAllEntities();
}
