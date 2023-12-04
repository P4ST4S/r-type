/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** IGraphic
*/

#ifndef IGAPHIC_HPP
#define IGAPHIC_HPP

#include <string>
#include <functional>
#include <any>
#include <cstdint>
#include <stdint.h>

namespace RGraphic {

    using RTexture = std::any;
    using RMusic = std::any;

    typedef struct RRectangle_s {
        double x;
        double y;
        double width;
        double height;
    } RRectangle_t;

    typedef struct RVect2_s {
        double x;
        double y;
    } RVect2_t;

    typedef struct RColor_s {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    } RColor_t;

    typedef struct RInputs {
        bool up;
        bool down;
        bool left;
        bool right;
        bool shoot;
        bool pause;
    } RInputs_t;

    enum Resolution {
        RESOLUTION_848_480,
        RESOLUTION_1280_720,
        RESOLUTION_1920_1080,
        RESOLUTION_2560_1440,
    };

    enum Fps {
        FPS_30 = 30,
        FPS_60 = 60,
        FPS_120 = 120,
    };

    class IGraphic {
        public:
            virtual ~IGraphic() = default;

            virtual void openWindow(std::size_t width, std::size_t height, std::string name) = 0;
            virtual void toggleFullscreen(bool fullscreen, int selectedResolution, std::string name) = 0;
            virtual void setFps(int fps) = 0;
            virtual void setVolume(double volume) = 0;
            virtual bool windowShouldClose() = 0;
            virtual void closeWindow() = 0;
            virtual void startFrame() = 0;
            virtual void endFrame() = 0;
            virtual void drawText(RVect2_t position, std::string text, uint16_t fontSize, uint16_t outlineSize, RColor_t color, RColor_t outlineColor) = 0;
            virtual void drawRectangle(RRectangle_t rectangle, RColor_t color) = 0;
            virtual void drawCheckbox(RRectangle_t rectangle, std::string text, bool &checked) = 0;
            virtual void drawInputBox(RRectangle_t rectangle, char *text, uint16_t maxBuf, bool &editMode) = 0;
            virtual void drawButton(RRectangle_t rectangle, std::string text, bool shown, std::function<void()> onClick) = 0;
            virtual void drawDropDownButton(RRectangle_t rectangle, std::string text, int &activeMode, bool &editMode) = 0;
            virtual void drawSlider(RRectangle_t rectangle, std::string textLeft, std::string textRight, double &value) = 0;
            virtual void drawTexture(RGraphic::RTexture rtexture, RRectangle_t source, RRectangle_t dest, double rotation) = 0;
            virtual RGraphic::RTexture loadTexture(std::string path) = 0;
            virtual void loadFont(std::string path) = 0;
            virtual RGraphic::RMusic loadMusic(std::string path) = 0;
            virtual void playMusic(RGraphic::RMusic rmusic) = 0;
            virtual void updateMusic(RGraphic::RMusic rmusic) = 0;
            virtual void unloadMusic(RGraphic::RMusic rmusic) = 0;
            virtual RInputs_t getInputs() = 0;
        protected:
        private:
    };
}

#endif
