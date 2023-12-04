/*
** EPITECH PROJECT, 2023
** EntityComponentSystem
** File description:
** ISystem
*/

#pragma once

#include "Zipper.hpp"

class Registry;

/**
 * @brief Interface of the systems of the ECS
 * 
 */
class ISystem
{
    public:
        virtual ~ISystem() = default;
        virtual void operator()(Registry &registry) = 0;
};
