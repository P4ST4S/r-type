/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** EndFrameSystem
*/

#pragma once

#include "ASystem.hpp"
#include "IGraphic.hpp"
#include "InputEventListener.hpp"

#include <memory>

class EndFrameSystem : public ASystem {
    public:
        EndFrameSystem(std::shared_ptr<RGraphic::IGraphic> lib);
        ~EndFrameSystem();

        void operator()(Registry &registry);

    protected:
    private:
        std::shared_ptr<RGraphic::IGraphic> _lib;
};

