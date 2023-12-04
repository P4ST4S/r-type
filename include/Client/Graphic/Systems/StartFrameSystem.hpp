/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** StartFrameSystem
*/

#pragma once

#include "ASystem.hpp"
#include "IGraphic.hpp"
#include "Clock.hpp"

#include <memory>

class StartFrameSystem : public ASystem {
    public:
        StartFrameSystem(std::shared_ptr<RGraphic::IGraphic> lib);
        ~StartFrameSystem();

        void operator()(Registry &registry);

    protected:
    private:
        std::shared_ptr<RGraphic::IGraphic> _lib;
        Clock _clock;
};
