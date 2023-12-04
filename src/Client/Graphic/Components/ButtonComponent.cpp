/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** ButtonComponent
*/

#include "ButtonComponent.hpp"

ButtonComponent::ButtonComponent(Vector2D<double> rect, std::string text, bool shown, std::function<void()> onClick) : _rect(rect), _text(text), _shown(shown), _onClick(onClick)
{
}

ButtonComponent::~ButtonComponent()
{
}

void ButtonComponent::setRectangle(Vector2D<double> rect)
{
    _rect = rect;
}

void ButtonComponent::setText(std::string text)
{
    _text = text;
}

void ButtonComponent::setShown(bool shown)
{
    _shown = shown;
}

void ButtonComponent::setOnClick(std::function<void()> onClick)
{
    _onClick = onClick;
}

Vector2D<double> ButtonComponent::getRectangle() const
{
    return (_rect);
}

std::string ButtonComponent::getText() const
{
    return (_text);
}

bool ButtonComponent::getShown() const
{
    return (_shown);
}

std::function<void()> ButtonComponent::getOnClick() const
{
    return (_onClick);
}

void destroyButton(Registry &registry, Entity const &entity)
{
    SparseArray<ButtonComponent> &buttonArray = registry.getComponents<ButtonComponent>();

    if (buttonArray.size() > entity)
        buttonArray[entity].reset();
}
