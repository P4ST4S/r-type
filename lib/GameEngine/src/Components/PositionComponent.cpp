/*
** EPITECH PROJECT, 2023
** GameEngine
** File description:
** PositionComponent
*/

#include "PositionComponent.hpp"

PositionComponent::PositionComponent(double x, double y) : _position(x, y)
{
}

PositionComponent::PositionComponent(Vector2D<double> position) : _position(position._x, position._y)
{
}

void PositionComponent::move(Vector2D<double> direction)
{
    _position._x += direction._x;
    _position._y += direction._y;
    // this->replacePosition();
}

void PositionComponent::setPosition(Vector2D<double> newPosition)
{
    _position._x = newPosition._x;
    _position._y = newPosition._y;
    // this->replacePosition();
}

Vector2D<double> PositionComponent::getPosition() const
{
    return (_position);
}

void PositionComponent::replacePosition(void)
{
    if (_position._x < 0)
        _position._x = 0;
    if (_position._x > INTERNAL_WIDTH)
        _position._x = INTERNAL_WIDTH;
    if (_position._y < 0)
        _position._y = 0;
    if (_position._y > INTERNAL_HEIGHT)
        _position._y = INTERNAL_HEIGHT;
}

void destroyPosition(Registry &registry, Entity const &entity)
{
    SparseArray<PositionComponent> &positionArray = registry.getComponents<PositionComponent>();

    if (positionArray.size() > entity)
        positionArray[entity].reset();
}
