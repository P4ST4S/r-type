/*
** EPITECH PROJECT, 2023
** GameEngine
** File description:
** HealthComponent
*/

#pragma once

#include "IComponent.hpp"

class HealthComponent {
    public:
        /**
         * @brief Construct a new Health for an entity
         * 
         * @param health max health of the entity
         */
        HealthComponent(std::size_t health);

        /**
         * @brief Compare two HealthComponent
         * 
         * @param other the other HealthComponent to compare with this
         * @return true if the two HealthComponent are the same
         * @return false if the two HealthComponent are different
         */
        bool operator==(const HealthComponent &other) const;
        
        /**
         * @brief Used to heal the entity
         * 
         * @param health The amount of health to heal
         */
        void heal();

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

    private:
        std::size_t _health; // Health of the entity
        std::size_t _maxHealth; // Max health of the entity based of the health entered in the constructor
};

/**
 * @brief Destroy the component HealthComponent of an entity
 * 
 * @param registry Registry of the game, used to destroy the component
 * @param entity Entity which will lose the component
 */
void destroyHealth(Registry &registry, Entity const &entity);
