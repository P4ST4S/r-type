/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** TextComponent
*/

#pragma once

#include "IComponent.hpp"
#include "IGraphic.hpp"

class TextComponent : public IComponent{
    public:
        TextComponent(std::string text, uint16_t fontSize, uint16_t outlineSize, RGraphic::RColor_t color, RGraphic::RColor_t outlineColor);
        ~TextComponent();

        void setText(std::string text);
        void setFontSize(uint16_t fontSize);
        void setOutlineSize(uint16_t outlineSize);
        void setColor(RGraphic::RColor_t color);
        void setOutlineColor(RGraphic::RColor_t outlineColor);

        std::string getText() const;
        uint16_t getFontSize() const;
        uint16_t getOutlineSize() const;
        RGraphic::RColor_t getColor() const;
        RGraphic::RColor_t getOutlineColor() const;

    private:
        std::string _text;
        uint16_t _fontSize;
        uint16_t _outlineSize;
        RGraphic::RColor_t _color;
        RGraphic::RColor_t _outlineColor;
};

void destroyText(Registry &registry, Entity const &entity);
