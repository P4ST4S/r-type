/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** MusicComponent
*/

#include "MusicComponent.hpp"

MusicComponent::MusicComponent(RGraphic::RMusic music) : _music(music)
{
}

MusicComponent::~MusicComponent()
{
}

RGraphic::RMusic MusicComponent::getMusic() const
{
    return (_music);
}

void destroyMusic(Registry &registry, Entity const &entity)
{
    SparseArray<MusicComponent> &musicArray = registry.getComponents<MusicComponent>();

    if (musicArray.size() > entity)
        musicArray[entity].reset();
}
