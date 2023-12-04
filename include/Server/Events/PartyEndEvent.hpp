/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** InputEventListener
*/

#pragma once

#include "IEventListener.hpp"
#include "IEvent.hpp"
#include "Registry.hpp"
#include "GameEngine.hpp"

#include <memory>

class PartyEndEvent : public IEvent{
    public:
        PartyEndEvent();
        PartyEndEvent(bool status);
        ~PartyEndEvent();

        bool getStatus() const;
        void setStatus(bool status);

        static void createPartyEnd(Registry &registry, bool status);
    private:
        bool _status = false;
};
