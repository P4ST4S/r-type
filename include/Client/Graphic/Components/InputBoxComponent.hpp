/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** InputBoxComponent
*/

#pragma once

#include "IComponent.hpp"

#include <cstring>

class InputBoxComponent : public IComponent {
    public:
        InputBoxComponent(Vector2D<double> rect, const char *text, uint16_t textSize);
        ~InputBoxComponent();

        void setRectangle(Vector2D<double> rect);
        void setText(const char *text);
        void setTextSize(uint16_t textSize);
        void setEditMode(bool editMode);

        Vector2D<double> getRectangle() const;
        char *getText();
        uint16_t getTextSize() const;
        bool &getEditMode();

    protected:
    private:
        Vector2D<double> _rect;
        char *_text;
        uint16_t _textSize;
        bool _editMode;
};

void destroyInputBox(Registry &registry, Entity const &entity);
