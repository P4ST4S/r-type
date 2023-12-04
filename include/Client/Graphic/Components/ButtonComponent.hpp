/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** ButtonComponent
*/

#pragma once

#include "IComponent.hpp"

class ButtonComponent : public IComponent{
    public:
        ButtonComponent(Vector2D<double> rect, std::string text, bool shown, std::function<void()> onClick);
        ~ButtonComponent();

        void setRectangle(Vector2D<double> rect);
        void setText(std::string text);
        void setShown(bool shown);
        void setOnClick(std::function<void()> onClick);

        Vector2D<double> getRectangle() const;
        std::string getText() const;
        bool getShown() const;
        std::function<void()> getOnClick() const;

    protected:
    private:
        Vector2D<double> _rect;
        std::string _text;
        bool _shown;
        std::function<void()> _onClick;
};

void destroyButton(Registry &registry, Entity const &entity);
