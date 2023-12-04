/*
** EPITECH PROJECT, 2023
** GameEngine
** File description:
** VelocityComponent
*/

#include "VelocityComponent.hpp"

VelocityComponent::VelocityComponent(double x, double y) : _velocity(x, y)
{
};

VelocityComponent::VelocityComponent(Vector2D<double> Velocity) : _velocity(Velocity)
{
};

Vector2D<double> VelocityComponent::getVelocity() const
{
    return (_velocity);
}

void destroyVelocity(Registry &registry, Entity const &entity)
{
    SparseArray<VelocityComponent> &velocityArray = registry.getComponents<VelocityComponent>();

    if (velocityArray.size() > entity)
        velocityArray[entity].reset();
}
