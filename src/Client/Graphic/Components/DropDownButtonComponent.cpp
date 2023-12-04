/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** DropDownButtonComponent
*/

#include "DropDownButtonComponent.hpp"

DropDownButtonComponent::DropDownButtonComponent(Vector2D<double> rect, std::string text, int &activeMode) : _rect(rect), _text(text), _activeMode(activeMode), _editMode(false)
{
}

DropDownButtonComponent::~DropDownButtonComponent()
{
}

void DropDownButtonComponent::setRectangle(Vector2D<double> rect)
{
    _rect = rect;
}

void DropDownButtonComponent::setText(std::string text)
{
    _text = text;
}

void DropDownButtonComponent::setActiveMode(int activeMode)
{
    _activeMode = activeMode;
}

void DropDownButtonComponent::setEditMode(bool editMode)
{
    _editMode = editMode;
}

Vector2D<double> DropDownButtonComponent::getRectangle() const
{
    return _rect;
}

std::string DropDownButtonComponent::getText() const
{
    return _text;
}

int &DropDownButtonComponent::getActiveMode()
{
    return _activeMode;
}

bool &DropDownButtonComponent::getEditMode()
{
    return _editMode;
}

void destroyDropDownButton(Registry &registry, Entity const &entity)
{
    SparseArray<DropDownButtonComponent> &dropDownButtonArray = registry.getComponents<DropDownButtonComponent>();

    if (dropDownButtonArray.size() > entity)
        dropDownButtonArray[entity].reset();
}
