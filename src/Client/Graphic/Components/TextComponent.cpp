/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** TextComponent
*/

#include "TextComponent.hpp"

TextComponent::TextComponent(std::string text, uint16_t fontSize, uint16_t outlineSize, RGraphic::RColor_t color, RGraphic::RColor_t outlineColor) : _text(text), _fontSize(fontSize), _outlineSize(outlineSize), _color(color), _outlineColor(outlineColor)
{
}

TextComponent::~TextComponent()
{
}

void TextComponent::setText(std::string text)
{
    _text = text;
}

void TextComponent::setFontSize(uint16_t fontSize)
{
    _fontSize = fontSize;
}

void TextComponent::setOutlineSize(uint16_t outlineSize)
{
    _outlineSize = outlineSize;
}

void TextComponent::setColor(RGraphic::RColor_t color)
{
    _color = color;
}

void TextComponent::setOutlineColor(RGraphic::RColor_t outlineColor)
{
    _outlineColor = outlineColor;
}

std::string TextComponent::getText() const
{
    return (_text);
}

uint16_t TextComponent::getFontSize() const
{
    return (_fontSize);
}

uint16_t TextComponent::getOutlineSize() const
{
    return (_outlineSize);
}

RGraphic::RColor_t TextComponent::getColor() const
{
    return (_color);
}

RGraphic::RColor_t TextComponent::getOutlineColor() const
{
    return (_outlineColor);
}

void destroyText(Registry &registry, Entity const &entity)
{
    SparseArray<TextComponent> &textArray = registry.getComponents<TextComponent>();

    if (textArray.size() > entity)
        textArray[entity].reset();
}
