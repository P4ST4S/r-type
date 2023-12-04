/*
** EPITECH PROJECT, 2023
** EntityComponentSystem
** File description:
** EventsWorker
*/

#include "EventsWorker.hpp"
#include "Registry.hpp"

EventsWorker::EventsWorker(Registry &regitry) : _registry(regitry)
{
}

void EventsWorker::operator()()
{
    while (_registry.isLooping()){
        for (auto &listener : _listeners)
            listener->handleEvents(_registry);
    }
}

void EventsWorker::addEventListener(std::shared_ptr<IEventListener> listener)
{
    _listeners.push_back(listener);
}