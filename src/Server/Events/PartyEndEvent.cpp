/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** PartyEndEvent
*/

#include "PartyEndEvent.hpp"

PartyEndEvent::PartyEndEvent()
{
}


PartyEndEvent::PartyEndEvent(bool status) : _status(status)
{
}

PartyEndEvent::~PartyEndEvent()
{
}

bool PartyEndEvent::getStatus() const
{
    return (_status);
}

void PartyEndEvent::setStatus(bool status)
{
    _status = status;
}


void PartyEndEvent::createPartyEnd(Registry &registry, bool status)
{
    registry.addEvent<PartyEndEvent>(PartyEndEvent(status));
}
