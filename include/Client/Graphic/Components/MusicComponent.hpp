/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** MusicComponents
*/

#pragma once

#include "IComponent.hpp"
#include "IGraphic.hpp"

class MusicComponent : public IComponent{
    public:
        MusicComponent(RGraphic::RMusic music);
        ~MusicComponent();

        RGraphic::RMusic getMusic() const;

    protected:
    private:
        RGraphic::RMusic _music;
};

void destroyMusic(Registry &registry, Entity const &entity);
