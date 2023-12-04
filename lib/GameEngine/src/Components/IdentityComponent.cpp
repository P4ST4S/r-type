/*
** EPITECH PROJECT, 2023
** GameEngine
** File description:
** IdentityComponent
*/

#include "IdentityComponent.hpp"

IdentityComponent::IdentityComponent(int identity) : _identity(identity)
{
}

int IdentityComponent::getIdentity() const
{
    return (_identity);
}

bool IdentityComponent::operator==(const IdentityComponent &other) const
{
    return (_identity == other.getIdentity());
}

void destroyIdentity(Registry &registry, Entity const &entity)
{
    SparseArray<IdentityComponent> &identityArray = registry.getComponents<IdentityComponent>();

    if (identityArray.size() > entity)
        identityArray[entity].reset();
}
