/*
** EPITECH PROJECT, 2023
** GameEngine
** File description:
** Collision
*/

#include "Collision.hpp"

namespace Geometry {
    bool rectanglesOverlap(Rectangle rect1, Rectangle rect2)
    {
        Rectangle box1 = {Vector2D<double>(rect1.pos._x - (rect1.size._x / 2), rect1.pos._y - (rect1.size._y / 2)), Vector2D<double>(rect1.size._x, rect1.size._y)};
        Rectangle box2 = {Vector2D<double>(rect2.pos._x - (rect2.size._x / 2), rect2.pos._y - (rect2.size._y / 2)), Vector2D<double>(rect2.size._x, rect2.size._y)};

        if (box1.pos._x < box2.pos._x + box2.size._x &&
            box1.pos._x + box1.size._x > box2.pos._x &&
            box1.pos._y < box2.pos._y + box2.size._y &&
            box1.pos._y + box1.size._y > box2.pos._y)
            return (true);
        return (false);
    }
}