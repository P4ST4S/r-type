/*
** EPITECH PROJECT, 2023
** EntityComponentSystem
** File description:
** Registry
*/

#include "Registry.hpp"
#include "CrossPlatformNetwork.hpp"

Registry::Registry() : _components(), _functions(), _freeIndices(), _entity_count(0), _systems(), _eventsWorker(*this), _isLooping(true)
{
};

Entity Registry::spawnEntity()
{
    if (_freeIndices.empty())
        return (Entity(_entity_count++));
    std::size_t idx = _freeIndices.back();
    _freeIndices.pop_back();
    return (Entity(idx));
}

Entity Registry::entityFromIndex(std::size_t idx)
{
    return (Entity(idx));
}

void Registry::killEntity(Entity const &entity)
{
    for (auto &func : _functions) {
        func(*this, entity);
    }
    _freeIndices.push_back(entity);
}


void Registry::killAllEntities()
{
    for (size_t i = 0; i < _entity_count; i++){
        if (std::find(_freeIndices.begin(), _freeIndices.end(), i) == _freeIndices.end())
            killEntity(entityFromIndex(i));
    }
    _entity_count = 0;
    _freeIndices.clear();
}

void Registry::runSystems()
{
    for (auto &system : _systems)
        (*system)(*this);
}

void Registry::runLoop()
{
    std::thread worker(_eventsWorker);

    while (_isLooping){
        runSystems();
    };
    worker.join();
}

void Registry::setLoop(bool status)
{
    _isLooping = status;
}

bool Registry::isLooping() const
{
    return (_isLooping);
}
