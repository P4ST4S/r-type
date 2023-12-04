/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** ClientEventComponent
*/

#pragma once

#include "IComponent.hpp"
#include "BodyStruct.hpp"

typedef struct PlayerEvents_s {
    bool _up;
    bool _down;
    bool _left;
    bool _right;
    bool _shoot;
} PlayerEvents_t;

class PlayerEventsComponent {
public:
    PlayerEventsComponent();
    ~PlayerEventsComponent();

    void updateEvent(const ClientMoveEvent_t &move);
    void updateEvent(const ClientShootEvent_t &shoot);

    const PlayerEvents_t &getEvents(void) const;

private:
    PlayerEvents_t _events;
};

void destroyPlayerEvents(Registry &registry, Entity const &entity);
