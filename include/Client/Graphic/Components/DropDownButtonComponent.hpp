/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** DropDownButtonComponent
*/

#pragma once

#include "IComponent.hpp"

class DropDownButtonComponent : public IComponent {
    public:
        DropDownButtonComponent(Vector2D<double> rect, std::string text, int &activeMode);
        ~DropDownButtonComponent();

        void setRectangle(Vector2D<double> rect);
        void setText(std::string text);
        void setActiveMode(int activeMode);
        void setEditMode(bool editMode);

        Vector2D<double> getRectangle() const;
        std::string getText() const;
        int &getActiveMode();
        bool &getEditMode();

    protected:
    private:
        Vector2D<double> _rect;
        std::string _text;
        int &_activeMode;
        bool _editMode;
};

void destroyDropDownButton(Registry &registry, Entity const &entity);
