/*
** EPITECH PROJECT, 2023
** EntityComponentSystem
** File description:
** Entity
*/

#pragma once

#include <cstddef>

/**
 * @class Entity
 * @brief Represents an entity in the Entity Component System.
 */
class Entity{
    friend class Registry;

    public:
        /**
         * @brief Implicit conversion operator to size_t.
         * 
         * @return The id of the entity.
         */
        operator size_t() const {
            return _id;
        }
    private:  
        /**
         * @brief Constructs an Entity object with a given id.
         * 
         * @param id The id of the entity.
         */
        explicit Entity(std::size_t id) : _id(id) {};

        std::size_t _id; // The id of the entity.
};
