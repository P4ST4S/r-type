/*
** EPITECH PROJECT, 2023
** EntityComponentSystem
** File description:
** EventsWorker
*/

#pragma once

#include "IEvent.hpp"
#include "IEventListener.hpp"

#include <vector>
#include <memory>

/**
 * @brief The EventsWorker class is responsible of running the enventListeners
 * 
 */
class EventsWorker
{
    friend class Registry;
    public:
        /**
         * @brief Construct a new Events Worker object
         * 
         * @param registry Registry of the game, 
         */
        EventsWorker(Registry &registry);

        /**
         * @brief Used to run the main loop of the EventsWorker which will call the listeners
         * 
         */
        void operator()();

        /**
         * @brief Add a new listener to the EventsWorker
         * 
         * @param listener Listener to add
         */
        void addEventListener(std::shared_ptr<IEventListener> listener);
    private:
        std::vector<std::shared_ptr<IEventListener>> _listeners; /** < List of the listeners of the EventsWorker */
        Registry &_registry; /** < Registry of the game, used to get the entities */
};
