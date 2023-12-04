/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** CheckboxComponent
*/

#pragma once

#include "IComponent.hpp"

class CheckboxComponent : public IComponent {
    public:
        CheckboxComponent(Vector2D<double> rect, std::string text, bool &checked);
        ~CheckboxComponent();

        void setRectangle(Vector2D<double> rect);
        void setText(std::string text);
        void setChecked(bool checked);

        Vector2D<double> getRectangle() const;
        std::string getText() const;
        bool &getChecked();

    protected:
    private:
        Vector2D<double> _rect;
        std::string _text;
        bool &_checked;
};

void destroyCheckbox(Registry &registry, Entity const &entity);
