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

class NewEntityEvent : public IEvent{
    public:
        NewEntityEvent();
        NewEntityEvent(size_t entityId, size_t _entityType);
        ~NewEntityEvent();

        size_t getEntityId() const;
        size_t getEntityType() const;
        void setEntityId(size_t entityId);
        void setEntityType(size_t entityType);

        /**
         ** @brief Create a new entity and create an event about it.
         **
         ** @param registry
         ** @param entityType
         ** @return Entity
         */
        static Entity createNewEntity(Registry &registry, size_t entityType);
    private:
        size_t _entityId = 0;
        size_t _entityType = 0;
};
