/*
** EPITECH PROJECT, 2023
** GameEngine
** File description:
** IdentityComponent
*/

#pragma once

#include "IComponent.hpp"


class IdentityComponent : public IComponent{
    public:
        /**
         * @brief Construct a new Identity Component
         * 
         * @param Identity Current Identity of the entity
         */
        IdentityComponent(int identity);


        bool operator==(const IdentityComponent &other) const;

        /**
         * @brief Get the Identity of the entity
         * 
         * @return int return the Identity of the entity
         */
        int getIdentity() const;
    private:
        int _identity; //Current Identity of the entity
};

/**
 * @brief Destroy the component IdentityComponent of an entity
 * 
 * @param registry Registry of the game, used to destroy the component
 * @param entity Entity which will lose the component
 */
void destroyIdentity(Registry &registry, Entity const &entity);