/*
** EPITECH PROJECT, 2023
** GameEngine
** File description:
** Rectangle
*/

#pragma once

#include "Vector2D.hpp"

namespace Geometry {
struct Rectangle
    {
        Vector2D<double> pos;
        Vector2D<double> size;
    };
}