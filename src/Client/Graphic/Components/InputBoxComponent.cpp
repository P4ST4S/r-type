/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** InputBoxComponent
*/

#include "InputBoxComponent.hpp"

InputBoxComponent::InputBoxComponent(Vector2D<double> rect, const char *holder = "", uint16_t textSize = 10) : _rect(rect),  _textSize(textSize), _editMode(false), _text(new char[textSize + 1])
{
    std::memset(this->_text, 0, textSize + 1);
    std::strncpy(this->_text, holder, textSize + 1);
}

InputBoxComponent::~InputBoxComponent()
{
}

void InputBoxComponent::setRectangle(Vector2D<double> rect)
{
    this->_rect = rect;
}

void InputBoxComponent::setText(const char *text)
{
    std::memset(this->_text, 0, this->_textSize + 1);
    std::strncpy(this->_text, text, this->_textSize + 1);
}

void InputBoxComponent::setTextSize(uint16_t textSize)
{
    this->_textSize = textSize;
}

void InputBoxComponent::setEditMode(bool editMode)
{
    this->_editMode = editMode;
}

Vector2D<double> InputBoxComponent::getRectangle() const
{
    return _rect;
}

char *InputBoxComponent::getText()
{
    return _text;
}

uint16_t InputBoxComponent::getTextSize() const
{
    return _textSize;
}

bool &InputBoxComponent::getEditMode()
{
    return _editMode;
}

void destroyInputBox(Registry &registry, Entity const &entity)
{
    SparseArray<InputBoxComponent> &inputBoxArray = registry.getComponents<InputBoxComponent>();

    if (inputBoxArray.size() > entity)
        inputBoxArray[entity].reset();
}
