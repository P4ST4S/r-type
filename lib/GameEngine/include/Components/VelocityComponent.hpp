/*
** EPITECH PROJECT, 2023
** GameEngine
** File description:
** VelocityComponent
*/

#pragma once

#include "IComponent.hpp"
#include "Vector2D.hpp"

class VelocityComponent : public IComponent{
    public:
        /**
         * @brief Construct a new Velocity Component
         * 
         * @param x velocity of the entity on the x axis
         * @param y velocity of the entity on the y axis
         */
        VelocityComponent(double x, double y);

        /**
         * @brief Construct a new Velocity Component
         * 
         * @param velocity velocity of the entity
         */
        VelocityComponent(Vector2D<double> Velocity);

        /**
         * @brief Get the Velocity of the entity
         * 
         * @return Vector2D<double> velocity of the entity
         */
        Vector2D<double> getVelocity() const;
    private:
        Vector2D<double> _velocity; // velocity of the entity
};

void destroyVelocity(Registry &registry, Entity const &entity);
