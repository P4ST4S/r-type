/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** main
*/

#include "Registry.hpp"
#include "GameEngine.hpp"
#include "RaylibGraphic.hpp"
#include "GraphicEngine.hpp"
#include "Scenes.hpp"
#include "GlobalSettings.hpp"

using namespace GlobalSettings;

int main(void)
{
    Registry registry;
    std::shared_ptr<RGraphic::IGraphic> sgraphic = std::make_shared<RGraphic::RaylibGraphic>(RGraphic::RaylibGraphic());
    RGraphic::RInputs_t inputs;
    CheckboxSystem checkboxSystem(sgraphic);
    ButtonSystem buttonSystem(sgraphic);
    DropDownButtonSystem dropDownButtonSystem(sgraphic);
    SliderSystem sliderSystem(sgraphic);
    TextSystem textSystem(sgraphic);
    InputBoxSystem inputBoxSystem(sgraphic);
    TextureSystem textureSystem(sgraphic);
    sgraphic->openWindow(_screenWidth, _screenHeight, "R-Type");
    SceneManagerSystem sceneManager(registry, sgraphic);

    registry.registerComponent<PositionComponent>(destroyPosition);
    registry.registerComponent<CheckboxComponent>(destroyCheckbox);
    registry.registerComponent<ButtonComponent>(destroyButton);
    registry.registerComponent<DropDownButtonComponent>(destroyDropDownButton);
    registry.registerComponent<SliderComponent>(destroySlider);
    registry.registerComponent<TextComponent>(destroyText);
    registry.registerComponent<InputBoxComponent>(destroyInputBox);
    registry.registerComponent<TextureComponent>(destroyTexture);
    registry.registerComponent<InterpolationComponent>(destroyInterpolation);

    registry.registerEvent<InputEvent>();

    registry.addEventListener(InputEventListener(sgraphic));

    registry.addSystem(StartFrameSystem(sgraphic));
    registry.addSystem<PositionComponent, TextureComponent>(textureSystem);
    registry.addSystem<PositionComponent, CheckboxComponent>(checkboxSystem);
    registry.addSystem<PositionComponent, ButtonComponent>(buttonSystem);
    registry.addSystem<PositionComponent, SliderComponent>(sliderSystem);
    registry.addSystem<PositionComponent, TextComponent>(textSystem);
    registry.addSystem<PositionComponent, InputBoxComponent>(inputBoxSystem);
    registry.addSystem<PositionComponent, DropDownButtonComponent>(dropDownButtonSystem);
    registry.addSystem(sceneManager);
    registry.addSystem<InterpolationComponent, PositionComponent>(InterpolationSystem(_screenWidth, _screenHeight));
    registry.addSystem(EndFrameSystem(sgraphic));

    sceneManager.addScene("TitleMenu", TitleMenuScene());
    sceneManager.addScene("MainMenu", MainMenuScene());
    sceneManager.addScene("SettingsMenu", SettingsMenuScene());
    sceneManager.addScene("ConnectToServerMenu", ConnectToServerMenuScene());
    sceneManager.addScene("ConnectingToServerMenu", ConnectingToServerMenuScene());
    sceneManager.addScene("ServerLobbyMenu", ServerLobbyMenuScene());
    sceneManager.addScene("GameLoading", GameLoadingScene());
    sceneManager.addScene("Game", GameScene());
    sceneManager.changeScene(registry, "TitleMenu");

    registry.runLoop();
    sgraphic->closeWindow();
    return (0);
}
