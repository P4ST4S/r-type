/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** SettingsMenuScene
*/

#include "SettingsMenuScene.hpp"

using namespace GlobalSettings;
using namespace RGraphic;

void SettingsMenuScene::loadScene(SceneManagerSystem &manager)
{
    Registry &registry = manager.getRegistry();
    RGraphic::IGraphic &lib = manager.getGraphic();
    manager.getClientPkgHandler().disconnect();

    Entity fullscreen = registry.spawnEntity();
    Entity fullscreenBackup = registry.spawnEntity();
    Entity volume = registry.spawnEntity();
    Entity fps = registry.spawnEntity();
    Entity resolution = registry.spawnEntity();
    Entity saveButton = registry.spawnEntity();
    Entity backButton = registry.spawnEntity();

    Vector2D<double> checkboxSize = {static_cast<double>(_screenHeight * 0.05), static_cast<double>(_screenHeight * 0.05)};
    Vector2D<double> dropDownButtonSize = {static_cast<double>(_screenWidth * 0.3), static_cast<double>(_screenHeight * 0.1)};
    Vector2D<double> buttonSize = {static_cast<double>(_screenWidth * 0.2), static_cast<double>(_screenHeight * 0.1)};
    Vector2D<double> sliderSize = {static_cast<double>(_screenWidth * 0.4), static_cast<double>(_screenHeight * 0.05)};
    double buttonSpacing = dropDownButtonSize._y * 1.5;
    Vector2D checkboxPos = {static_cast<float>((_screenWidth - checkboxSize._x) / 2), static_cast<float>((_screenHeight - checkboxSize._x) / 2)};
    Vector2D buttonPos = {static_cast<float>((_screenWidth - dropDownButtonSize._x) / 2), static_cast<float>((_screenHeight - dropDownButtonSize._x) / 2)};
    Vector2D sliderPos = {static_cast<float>((_screenWidth - sliderSize._x) / 2), static_cast<float>((_screenHeight - sliderSize._x) / 2)};

    registry.addComponent<PositionComponent>(fullscreen, PositionComponent(checkboxPos._x, buttonPos._y));
    registry.addComponent<PositionComponent>(fullscreenBackup, PositionComponent(checkboxPos._x + checkboxSize._x + buttonSpacing, buttonPos._y));
    registry.addComponent<PositionComponent>(resolution, PositionComponent(buttonPos._x, buttonPos._y + buttonSpacing));
    registry.addComponent<PositionComponent>(fps, PositionComponent(buttonPos._x, buttonPos._y + buttonSpacing * 2));
    registry.addComponent<PositionComponent>(volume, PositionComponent(sliderPos._x, buttonPos._y + buttonSpacing * 3));
    registry.addComponent<PositionComponent>(saveButton, PositionComponent(_screenWidth * 0.95 - buttonSize._x, _screenHeight * 0.95 - buttonSize._y));
    registry.addComponent<PositionComponent>(backButton, PositionComponent(_screenWidth * 0.05, _screenHeight * 0.05));

    registry.addComponent<CheckboxComponent>(fullscreen, CheckboxComponent(checkboxSize, "FULLSCREEN", _fullscreen));
    registry.addComponent<ButtonComponent>(fullscreenBackup, ButtonComponent(buttonSize, "FULLSCREEN", true, [&]() {
        _fullscreen = !_fullscreen;
    }));
    registry.addComponent<DropDownButtonComponent>(resolution, DropDownButtonComponent(dropDownButtonSize, "848 X 480;1280 X 720;1920 X 1080;2560 X 1440", _selectedResolution));
    registry.addComponent<DropDownButtonComponent>(fps, DropDownButtonComponent(dropDownButtonSize, "30FPS;60 FPS;120 FPS", _selectedFps));
    registry.addComponent<SliderComponent>(volume, SliderComponent(sliderSize, "", "VOLUME", _volume));
    registry.addComponent<ButtonComponent>(saveButton, ButtonComponent(buttonSize, "SAVE", true, [&]() {
        manager.getGraphic().toggleFullscreen(_fullscreen, _selectedResolution, "R-TYPE");
        manager.getGraphic().setVolume(_volume);
        manager.getGraphic().setFps(_selectedFps);
        switch (_selectedResolution) {
            case RESOLUTION_848_480:
                _screenWidth = 848;
                _screenHeight = 480;
                break;
            case RESOLUTION_1280_720:
                _screenWidth = 1280;
                _screenHeight = 720;
                break;
            case RESOLUTION_1920_1080:
                _screenWidth = 1920;
                _screenHeight = 1080;
                break;
            case RESOLUTION_2560_1440:
                _screenWidth = 2560;
                _screenHeight = 1440;
                break;
        }
        manager.changeScene(registry, "SettingsMenu");
    }));
    registry.addComponent<ButtonComponent>(backButton, ButtonComponent(dropDownButtonSize, "BACK", true, [&]() {
        manager.changeScene(registry, "MainMenu");
    }));
}

void SettingsMenuScene::run(SceneManagerSystem &manager)
{
}

void SettingsMenuScene::unloadScene(SceneManagerSystem &manager)
{
    manager.getRegistry().killAllEntities();
}
