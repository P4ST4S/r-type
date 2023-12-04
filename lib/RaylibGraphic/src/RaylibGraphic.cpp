/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** RaylibGraphic
*/

#include "RaylibGraphic.hpp"

RGraphic::RaylibGraphic::RaylibGraphic()
{
}

RGraphic::RaylibGraphic::~RaylibGraphic()
{
}

void RGraphic::RaylibGraphic::openWindow(std::size_t width, std::size_t height, std::string name)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(width, height, name.c_str());
    SetExitKey(KEY_NULL);
    InitAudioDevice();
    SetTargetFPS(FPS_60);
    loadFont("assets/fonts/madness_hyperactive.otf");
}
#include <iostream>
void RGraphic::RaylibGraphic::toggleFullscreen(bool fullscreen, int selectedResolution, std::string name)
{
    if (!IsWindowFullscreen() && fullscreen) {
        int monitor = GetCurrentMonitor();
        std::cout << GetMonitorWidth(monitor) << " " << GetMonitorHeight(monitor) << std::endl;
        SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
        ToggleFullscreen();
    } else if (!fullscreen) {
        if (IsWindowFullscreen())
            ToggleFullscreen();
        switch (selectedResolution) {
            case RESOLUTION_848_480:
                SetWindowSize(848, 480);
                break;
            case RESOLUTION_1280_720:
                SetWindowSize(1280, 720);
                break;
            case RESOLUTION_1920_1080:
                SetWindowSize(1920, 1080);
                break;
            case RESOLUTION_2560_1440:
                SetWindowSize(2560, 1440);
                break;
            default:
                break;
        }
    }
}

void RGraphic::RaylibGraphic::setFps(int fps)
{
    switch (fps)
    {
    case FPS_30:
        SetTargetFPS(30);
        break;
    case FPS_60:
        SetTargetFPS(60);
        break;
    case FPS_120:
        SetTargetFPS(120);
        break;
    default:
        break;
    }
}

void RGraphic::RaylibGraphic::setVolume(double volume)
{
    SetMasterVolume(static_cast<float>(volume));
}

bool RGraphic::RaylibGraphic::windowShouldClose()
{
    return (WindowShouldClose());
}

void RGraphic::RaylibGraphic::closeWindow()
{
    UnloadFont(_font);
    CloseAudioDevice();
    CloseWindow();
}

void RGraphic::RaylibGraphic::startFrame()
{
    BeginDrawing();
    ClearBackground(BLACK);
}

void RGraphic::RaylibGraphic::endFrame()
{
    EndDrawing();
}

void RGraphic::RaylibGraphic::drawText(RVect2_t position, std::string text, uint16_t fontSize, uint16_t outlineSize, RColor_t color, RColor_t outlineColor)
{
    Vector2 position2 = {static_cast<float>(position.x), static_cast<float>(position.y)};
    Color color2 = {color.r, color.g, color.b, color.a};
    Color outlineColor2 = {outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a};

    if (outlineSize != 0) {
        DrawTextEx(_font, text.c_str(), {position2.x - outlineSize, position2.y - outlineSize}, fontSize, _font.glyphPadding, outlineColor2);
        DrawTextEx(_font, text.c_str(), {position2.x + outlineSize, position2.y - outlineSize}, fontSize, _font.glyphPadding, outlineColor2);
        DrawTextEx(_font, text.c_str(), {position2.x - outlineSize, position2.y + outlineSize}, fontSize, _font.glyphPadding, outlineColor2);
        DrawTextEx(_font, text.c_str(), {position2.x + outlineSize, position2.y + outlineSize}, fontSize, _font.glyphPadding, outlineColor2);
    }
    DrawTextEx(_font, text.c_str(), position2, fontSize, _font.glyphPadding, color2);
}

void RGraphic::RaylibGraphic::drawRectangle(RRectangle_t rectangle, RColor_t color)
{
    Rectangle rectangle2 = {static_cast<float>(rectangle.x), static_cast<float>(rectangle.y), static_cast<float>(rectangle.width), static_cast<float>(rectangle.height)};
    Color color2 = {color.r, color.g, color.b, color.a};

    DrawRectangleRec(rectangle2, color2);
}

void RGraphic::RaylibGraphic::drawCheckbox(RRectangle_t rectangle, std::string text, bool &checked)
{
    Rectangle rectangle2 = {static_cast<float>(rectangle.x), static_cast<float>(rectangle.y), static_cast<float>(rectangle.width), static_cast<float>(rectangle.height)};

    if (GuiCheckBox(rectangle2, text.c_str(), &checked))
        checked = !checked;
}

void RGraphic::RaylibGraphic::drawInputBox(RRectangle_t rectangle, char *text, uint16_t maxBuf, bool &editMode)
{
    Rectangle rectangle2 = {static_cast<float>(rectangle.x), static_cast<float>(rectangle.y), static_cast<float>(rectangle.width), static_cast<float>(rectangle.height)};

    if (GuiTextBox(rectangle2, text, maxBuf, editMode))
        editMode = !editMode;
}

void RGraphic::RaylibGraphic::drawButton(RRectangle_t rectangle, std::string text, bool shown, std::function<void()> onClick)
{
    if (!shown)
        return;
    Rectangle rectangle2 = {static_cast<float>(rectangle.x), static_cast<float>(rectangle.y), static_cast<float>(rectangle.width), static_cast<float>(rectangle.height)};

    if (GuiButton(rectangle2, text.c_str())) {
        onClick();
    }
}

void RGraphic::RaylibGraphic::drawDropDownButton(RRectangle_t rectangle, std::string text, int &activeMode, bool &editMode)
{
    Rectangle rectangle2 = {static_cast<float>(rectangle.x), static_cast<float>(rectangle.y), static_cast<float>(rectangle.width), static_cast<float>(rectangle.height)};


    if (GuiDropdownBox(rectangle2, text.c_str(), &activeMode, editMode))
        editMode = !editMode;
}

void RGraphic::RaylibGraphic::drawSlider(RRectangle_t rectangle, std::string textLeft, std::string textRight, double &value)
{
    Rectangle rectangle2 = {static_cast<float>(rectangle.x), static_cast<float>(rectangle.y), static_cast<float>(rectangle.width), static_cast<float>(rectangle.height)};
    float value2 = static_cast<float>(value);

    GuiSlider(rectangle2, textLeft.c_str(), textRight.c_str(), &value2, 0, 100);
    value = static_cast<double>(value2);
}

void RGraphic::RaylibGraphic::drawTexture(RGraphic::RTexture rtexture, RRectangle_t source, RRectangle_t dest, double rotation)
{
    Texture2D texture = std::any_cast<Texture2D>(rtexture);
    Rectangle sourceRec = {static_cast<float>(source.x), static_cast<float>(source.y), static_cast<float>(source.width), static_cast<float>(source.height)};
    Rectangle destRec = {static_cast<float>(dest.x), static_cast<float>(dest.y), static_cast<float>(dest.width), static_cast<float>(dest.height)};
    Vector2 origin = {0, 0};
    float rotation2 = rotation;
    Color color = WHITE;

    DrawTexturePro(texture, sourceRec, destRec, origin, rotation2, color);
}

RGraphic::RTexture RGraphic::RaylibGraphic::loadTexture(std::string path)
{
    Texture2D texture = LoadTexture(path.c_str());

    return (std::make_any<Texture2D>(texture));
}

void RGraphic::RaylibGraphic::loadFont(std::string path)
{
    _font = LoadFont(path.c_str());
}

RGraphic::RMusic RGraphic::RaylibGraphic::loadMusic(std::string path)
{
    Music music = LoadMusicStream(path.c_str());

    return (std::make_any<Music>(music));
}

void RGraphic::RaylibGraphic::playMusic(RGraphic::RMusic rmusic)
{
    Music music = std::any_cast<Music>(rmusic);

    PlayMusicStream(music);
}

void RGraphic::RaylibGraphic::updateMusic(RGraphic::RMusic rmusic)
{
    Music music = std::any_cast<Music>(rmusic);

    UpdateMusicStream(music);
}

void RGraphic::RaylibGraphic::unloadMusic(RGraphic::RMusic rmusic)
{
    Music music = std::any_cast<Music>(rmusic);

    UnloadMusicStream(music);
}

RGraphic::RInputs_t RGraphic::RaylibGraphic::getInputs()
{
    RInputs_t inputs;

    inputs.up = IsKeyDown(KEY_UP) || IsKeyDown(KEY_W);
    inputs.down = IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);
    inputs.left = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);
    inputs.right = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);
    inputs.shoot = IsKeyDown(KEY_SPACE) || IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    inputs.pause = IsKeyPressed(KEY_ESCAPE);
    return (inputs);
}
