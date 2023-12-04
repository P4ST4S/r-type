/*
** EPITECH PROJECT, 2023
** EntityComponentSystem
** File description:
** Icomponent
*/

#pragma once

#include <string>

#include "Registry.hpp"
#include "GameMacro.hpp"
#include "Rectangle.hpp"
#include <string>

#define INTERNAL_WIDTH 3840
#define INTERNAL_HEIGHT 2160

/**
 * @brief Interface of the components of the ECS
 *
 */
class IComponent {
    public:
        virtual ~IComponent() = default;
};
