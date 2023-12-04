/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** TextureComponent
*/

#pragma once

#include "IComponent.hpp"
#include "IGraphic.hpp"

#include <map>

class TextureComponent : public IComponent {
    public:
        struct TextureComponentData {
            RGraphic::RTexture texture;
            RGraphic::RVect2_t dimension;
            uint8_t frameRate;
            uint16_t nframes;
            double scale;
        };

        TextureComponent();
        ~TextureComponent();

        void setDefaultTexture(int id);
        void addTexture(int id, RGraphic::RTexture texture, RGraphic::RVect2_t dimension, uint16_t nframes, double scale, uint8_t frameRate = 24);

        void playTexture(int id);
        void nextFrame();
        void computeSourcePosition();

        RGraphic::RTexture getTexture() const;
        RGraphic::RRectangle_t getSourceRect() const;
        double getScale() const;
        uint16_t getNFrames() const;
        uint16_t getCurrentFrame() const;
        int getCurrentId() const;
        bool hadTexture() const;

    private:
        int _default;
        int _current;
        std::map<int, TextureComponentData> _textures;

        RGraphic::RTexture _texture;
        RGraphic::RRectangle_t _sourceRect;
        double _scale;
        uint8_t _frameRate = 10;
        uint16_t _nframes;
        uint16_t _frame;

        std::chrono::_V2::system_clock::time_point _gap = std::chrono::high_resolution_clock::now();
        std::chrono::_V2::system_clock::time_point _prevGap = std::chrono::high_resolution_clock::now();
        std::chrono::duration<size_t, std::nano> _duration = this->_gap - this->_prevGap;
        std::chrono::duration<size_t, std::nano> _realDuration = this->_gap - this->_prevGap;
};

void destroyTexture(Registry &registry, Entity const &entity);
