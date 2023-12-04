/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** SoundSystem
*/

#pragma once

#include "ASystem.hpp"
#include "IAudio.hpp"

#include <memory>

class SoundSystem : public ASystem {
    public:
        SoundSystem(std::shared_ptr<RAudio::IAudio> &lib);
        ~SoundSystem();

        void play();
        void pause();
        void stop();

    protected:
    private:
        std::shared_ptr<RAudio::IAudio> _lib;
};
