/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** DamageHitboxComponent
*/

#pragma once

#include "IComponent.hpp"

class DamageHitboxComponent : public IComponent{
    public:
        /**
         * @brief Construct a new Hitbox Component object
         * 
         * @param width witdh of the hitbox
         * @param height height of the hitbox
         */
        DamageHitboxComponent(double width, double height, std::size_t damages);

        /**
         * @brief Construct a new Hitbox Component object
         * 
         * @param size size of the hitbox
         * @param damages damages of the hitbox
         */
        DamageHitboxComponent(Vector2D<double> size, std::size_t damages);

        /**
         * @brief Get the Size object
         * 
         * @return return the size of the hitbox
         */
        Vector2D<double> getSize() const;

        /**
         * @brief Get the Damages object
         * 
         * @return return the damages of the hitbox
         */
        std::size_t getDamages() const;
        
        /**
         * @brief Set the Hit object
         * 
         * @param entity entity which has been hit
         */
        void setTouch(bool status);
        
        /**
         * @brief Verify if the hitbox has hit something
         * 
         * @return return if the hitbox has hit something
         */
        bool hasTouch() const;

    private:
        Vector2D<double> _size; //size of the hitbox
        std::size_t _damages; //damages of the hitbox
        bool _touch; //if the hitbox has hit something
};

/**
 * @brief Destroy the Component DamageHitboxComponent object
 * 
 * @param registry Registry of the game, used to destroy the component
 * @param entity Entity which will lose the component
 */
void destroyDamageHitbox(Registry &registry, Entity const &entity);