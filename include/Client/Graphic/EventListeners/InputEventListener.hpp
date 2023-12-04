/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** InputEventListener
*/

#pragma once

#include "IEventListener.hpp"
#include "IEvent.hpp"
#include "IGraphic.hpp"
#include "Registry.hpp"
#include "Clock.hpp"

#include <memory>

class InputEvent : public IEvent{
    public:
        InputEvent(RGraphic::RInputs_t inputs);

        RGraphic::RInputs_t getInputs() const;
    private:
        RGraphic::RInputs_t _inputs;    
};

class InputEventListener : public IEventListener{
    public:
        InputEventListener(std::shared_ptr<RGraphic::IGraphic> &graphic);
    
        void handleEvents(Registry &registry) override;
    private:
        std::shared_ptr<RGraphic::IGraphic> _graphic;
        Clock _clock;
};
