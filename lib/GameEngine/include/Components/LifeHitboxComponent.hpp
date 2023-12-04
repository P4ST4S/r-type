/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** LifeHitboxComponent
*/

#pragma once

#include "IComponent.hpp"
#include "DamageHitboxComponent.hpp"

class LifeHitboxComponent : public IComponent{
    public:
        /**
         * @brief Construct a new Hitbox Component object
         * 
         * @param width witdh of the hitbox
         * @param height height of the hitbox
         * @param health Health of the hitbox
         */
        LifeHitboxComponent(double width, double height, std::size_t health);

        /**
         * @brief Construct a new Hitbox Component object
         * 
         * @param size Size of the hitbox
         * @param health Health of the hitbox
         */
        LifeHitboxComponent(Vector2D<double> size, std::size_t health);

        /**
         * @brief Get the Size object
         * 
         * @return return the size of the hitbox
         */
        Vector2D<double> getSize() const;
    
        /**
         * @brief Used to heal the entity
         * 
         * @param health The amount of health to heal
         */
        void heal(std::size_t health);

        /**
         * @brief Used when the entity take damages
         * 
         * @param damages The amount of damages to take
         */
        void takeDamages(std::size_t damages);

        /**
         * @brief check if the entity is alive
         * 
         * @return true the entity is alive
         * @return false the entity is dead
         */
        bool isAlive();
        
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
        std::size_t _health; //health of the hitbox
        std::size_t _maxHealth; //max health of the hitbox
        bool _touch; //if the hitbox has hit something
};

/**
 * @brief Destroy the Component LifeHitboxComponent object
 * 
 * @param registry Registry of the game, used to destroy the component
 * @param entity Entity which will lose the component
 */
void destroyLifeHitbox(Registry &registry, Entity const &entity);
