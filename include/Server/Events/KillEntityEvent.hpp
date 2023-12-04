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

class KillEntityEvent : public IEvent{
    public:
        KillEntityEvent();
        KillEntityEvent(size_t entityId);
        ~KillEntityEvent();

        size_t getEntityId() const;
        void setEntityId(size_t entityId);

        /**
         ** @brief Create an event to kill the entity
         **
         ** @param registry
         ** @param entityId
         */
        static void killEntity(Registry &registry, size_t entityId);
    private:
        size_t _entityId = 0;
};
