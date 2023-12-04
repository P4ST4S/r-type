/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** RaylibGraphic
*/

#pragma once

#include "IGraphic.hpp"

#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

namespace RGraphic {

    class RaylibGraphic : public IGraphic {
        public:
            RaylibGraphic();
            ~RaylibGraphic();

            void openWindow(std::size_t width, std::size_t height, std::string name);
            void toggleFullscreen(bool fullscreen, int selectedResolution, std::string name);
            void setFps(int fps);
            void setVolume(double volume);
            bool windowShouldClose();
            void closeWindow();

            void startFrame();
            void endFrame();

            void drawText(RVect2_t position, std::string text, uint16_t fontSize, uint16_t outlineSize, RColor_t color, RColor_t outlineColor);
            void drawRectangle(RRectangle_t rectangle, RColor_t color);
            void drawCheckbox(RRectangle_t rectangle, std::string text, bool &checked);
            void drawInputBox(RRectangle_t rectangle, char *text, uint16_t maxBuf, bool &editMode);
            void drawButton(RRectangle_t rectangle, std::string text, bool shown, std::function<void()> onClick);
            void drawDropDownButton(RRectangle_t rectangle, std::string text, int &activeMode, bool &editMode);
            void drawSlider(RRectangle_t rectangle, std::string textLeft, std::string textRight, double &value);
            void drawTexture(RGraphic::RTexture rtexture, RRectangle_t source, RRectangle_t dest, double rotation);

            RGraphic::RTexture loadTexture(std::string path);
            void loadFont(std::string path);
            RGraphic::RMusic loadMusic(std::string path);

            void playMusic(RGraphic::RMusic rmusic);
            void updateMusic(RGraphic::RMusic rmusic);
            void unloadMusic(RGraphic::RMusic rmusic);
            RGraphic::RInputs_t getInputs();

        protected:
        private:
            Font _font;
    };
};
