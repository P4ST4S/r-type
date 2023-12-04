/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** CheckboxComponent
*/

#include "CheckboxComponent.hpp"

CheckboxComponent::CheckboxComponent(Vector2D<double> rect, std::string text, bool &checked) : _rect(rect), _text(text), _checked(checked)
{
}

CheckboxComponent::~CheckboxComponent()
{
}

void CheckboxComponent::setRectangle(Vector2D<double> rect)
{
    _rect = rect;
}

void CheckboxComponent::setText(std::string text)
{
    _text = text;
}

void CheckboxComponent::setChecked(bool checked)
{
    _checked = checked;
}

Vector2D<double> CheckboxComponent::getRectangle() const
{
    return _rect;
}

std::string CheckboxComponent::getText() const
{
    return _text;
}

bool &CheckboxComponent::getChecked()
{
    return _checked;
}

void destroyCheckbox(Registry &registry, Entity const &entity)
{
    SparseArray<CheckboxComponent> &checkboxArray = registry.getComponents<CheckboxComponent>();

    if (checkboxArray.size() > entity)
        checkboxArray[entity].reset();
}
