/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** SliderComponent
*/

#pragma once

#include "IComponent.hpp"

class SliderComponent : public IComponent {
    public:
        SliderComponent(Vector2D<double> rect, std::string textLeft, std::string textRight, double &value);
        ~SliderComponent();

        void setRectangle(Vector2D<double> rect);
        void setTextLeft(std::string text);
        void setTextRight(std::string text);
        void setValue(double value);

        Vector2D<double> getRectangle() const;
        std::string getTextLeft() const;
        std::string getTextRight() const;
        double &getValue();

    protected:
    private:
        Vector2D<double> _rect;
        std::string _textLeft;
        std::string _textRight;
        double &_value;
};

void destroySlider(Registry &registry, Entity const &entity);
