/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** SliderComponent
*/

#include "SliderComponent.hpp"

SliderComponent::SliderComponent(Vector2D<double> rect, std::string textLeft, std::string textRight, double &value) : _rect(rect), _textLeft(textLeft), _textRight(textRight), _value(value)
{
}

SliderComponent::~SliderComponent()
{
}

void SliderComponent::setRectangle(Vector2D<double> rect)
{
    _rect = rect;
}

void SliderComponent::setTextLeft(std::string text)
{
    _textLeft = text;
}

void SliderComponent::setTextRight(std::string text)
{
    _textRight = text;
}

void SliderComponent::setValue(double value)
{
    if (value < 0)
        _value = 0;
    else if (value > 100)
        _value = 100;
    else if (value >= 0 && value <= 100)
        _value = value;
}

Vector2D<double> SliderComponent::getRectangle() const
{
    return _rect;
}

std::string SliderComponent::getTextLeft() const
{
    return _textLeft;
}

std::string SliderComponent::getTextRight() const
{
    return _textRight;
}

double &SliderComponent::getValue()
{
    return _value;
}

void destroySlider(Registry &registry, Entity const &entity)
{
    SparseArray<SliderComponent> &SliderArray = registry.getComponents<SliderComponent>();

    if (SliderArray.size() > entity)
        SliderArray[entity].reset();
}
