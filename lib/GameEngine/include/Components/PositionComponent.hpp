/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** PositionComponent
*/

#pragma once

#include "IComponent.hpp"
#include "Vector2D.hpp"

class PositionComponent : public IComponent{
    public:
        /**
         * @brief Construct a new Position Component object
         *
         * @param x x position
         * @param y y position
         */
        PositionComponent(double x, double y);

        /**
         * @brief Construct a new Position Component object
         *
         * @param position position of the entity
         */
        PositionComponent(Vector2D<double> position);

        /**
         * @brief Move the entity in the direction of the vector
         *
         * @param direction direction where the entity will move
         */
        void move(Vector2D<double> direction);

        /**
         * @brief Set the Position object
         *
         * @param newPosition new position of the entity
         */
        void setPosition(Vector2D<double> newPosition);

        /**
         * @brief Get the Position object
         *
         * @return Vector2D<double> position of the entity
         */
        Vector2D<double> getPosition() const;
    private:
        void replacePosition(void);

        Vector2D<double> _position; //position of the entity
};

/**
 * @brief Destroy the Component Position object
 *
 * @param registry Registry of the game, used to destroy the component
 * @param entity Entity which will lose the component
 */
void destroyPosition(Registry &registry, Entity const &entity);
