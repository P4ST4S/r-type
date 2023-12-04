/*
** EPITECH PROJECT, 2023
** EntityComponentSystem
** File description:
** Registry
*/

#pragma once

#include "SparseArray.hpp"
#include "Entity.hpp"
#include "ISystem.hpp"
#include "EventsWorker.hpp"

#include <unordered_map>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <typeindex>
#include <memory>
#include <any>
#include <functional>

/**
 * @class Registry
 * @brief The Registry class is responsible for managing entities and their components.
 */
class Registry{
    public:
        /**
         * @brief Constructs a new Registry object.
         */
        Registry();

        /**
         * @brief Registers a component type with the registry.
         * @tparam Component The type of the component to register.
         * @param func A function to be called when a component of this type is added to an entity.
         * @return A reference to the SparseArray containing the registered components.
         */
        template <class Component>
        SparseArray<Component> &registerComponent(std::function<void(Registry &, Entity const &)> func)
        {
            _components[std::type_index(typeid(Component))] = std::make_any<SparseArray<Component>>();
            _functions.push_back(func);
            return (std::any_cast<SparseArray<Component>&>(_components[std::type_index(typeid(Component))]));
        };
        
        /**
         * @brief Gets a reference to the SparseArray containing components of the specified type.
         * @tparam Component The type of the component to get.
         * @return A reference to the SparseArray containing components of the specified type.
         */
        template <class Component>
        SparseArray<Component> &getComponents()
        {
            return (std::any_cast<SparseArray<Component>&>(_components[std::type_index(typeid(Component))]));
        };

        /**
         * @brief Gets a const reference to the SparseArray containing components of the specified type.
         * @tparam Component The type of the component to get.
         * @return A const reference to the SparseArray containing components of the specified type.
         */
        template <class Component>
        SparseArray<Component> const &getComponents() const
        {
            SparseArray<Component> &comp = getComponents<Component>();
            return (comp);
        };

        /**
         * @brief Spawns a new entity.
         * @return The newly spawned entity.
         */
        Entity spawnEntity();

        /**
         * @brief Gets an entity from its index.
         * @param idx The index of the entity to get.
         * @return The entity with the specified index.
         */
        Entity entityFromIndex(std::size_t idx);

        /**
         * @brief Kills an entity.
         * @param entity The entity to kill.
         */
        void killEntity(Entity const &entity);

        /**
         * @brief Kills every entities of the registry.
         */
        void killAllEntities();

        /**
         * @brief Adds a component to an entity.
         * @tparam Component The type of the component to add.
         * @param entity The entity to add the component to.
         * @param c A universal reference of the component to add.
         * @return A reference to the added component.
         */
        template <typename Component>
        typename SparseArray<Component>::reference_type addComponent(Entity const &entity, Component &&c)
        {
            return (getComponents<Component>().insertAt(entity, std::forward<Component>(c)));
        }

        /**
         * @brief Emplaces a component in an entity.
         * @tparam Component The type of the component to emplace.
         * @tparam Params The types of the parameters to forward to the component constructor.
         * @param entity The entity to emplace the component in.
         * @param params The parameters to forward to the component constructor.
         * @return A reference to the emplaced component.
         */
        template <typename Component, typename... Params>
        typename SparseArray<Component>::reference_type emplaceComponent(Entity const &entity, Params &&... params)
        {
            return (getComponents<Component>().emplaceAt(entity, std::forward<Params>(params)...));
        }

        /**
         * @brief Removes a component from an entity.
         * @tparam Component The type of the component to remove.
         * @param entity The entity to remove the component from.
         */
        template <typename Component>
        void removeComponent(Entity const &entity)
        {
            getComponents<Component>()[entity].reset();
        }

        /**
         * @brief Adds a system to the registry.
         * @tparam Components The types of the components the system requires.
         * @tparam Function The type of the function to add as a system.
         * @param f The function to add as a system.
         */
        template <class ... Components, typename Function>
        void addSystem(Function && f)
        {
            class FunctorSystem : public ISystem {
                Function fn_;
            public:
                FunctorSystem(Function fn) : fn_(fn) {}
                virtual void operator()(Registry &r) override {
                    fn_(r, r.getComponents<Components>()...);
                }
            };

            _systems.emplace_back(std::make_shared<FunctorSystem>(std::forward<Function>(f)));
        }

        /**
         * @brief Runs all systems in the registry.
         */
        void runSystems();

        /**
         * @brief Adds an event listener to the EventWorker of the registry.
         * @tparam Listener The type of the listener to add.
         * @param listener The listener to add.
         */
        template <typename Listener>
        void addEventListener(Listener &&listener)
        {
            _eventsWorker.addEventListener(std::make_shared<Listener>(std::forward<Listener>(listener)));
        }

        /**
         * @brief Get a copy of the events specified in the template parameter.
         * 
         * @tparam Event Event type queue to get
         * @return std::deque<Event> 
         */
        template <typename Event>
        std::deque<Event> getEvent()
        {
            std::deque<Event> copy;

            _eventMutex.lock();
            copy = std::any_cast<std::deque<Event>&>(_events[std::type_index(typeid(Event))]);
            _eventMutex.unlock();
            return (copy);
        }

        /**
         * @brief Clear the events specified in the template parameter.
         * 
         * @tparam Event Event type queue to clear
         */        
        template <typename Event>
        void clearEvent()
        {
            _eventMutex.lock();
            std::any_cast<std::deque<Event>&>(_events[std::type_index(typeid(Event))]).clear();
            _eventMutex.unlock();
        }

        /**
         * @brief Register an event type in the registry.
         * 
         * @tparam Event Event type to register
         */
        template <typename Event>
        void registerEvent()
        {
            _eventMutex.lock();
            _events[std::type_index(typeid(Event))] = std::make_any<std::deque<Event>>();
            _eventMutex.unlock();
        };

        /**
         * @brief Add an event to the event queue.
         * 
         * @tparam Event Event type to add
         * @param event Event to add
         */
        template <typename Event>
        void addEvent(Event &&event)
        {
            _eventMutex.lock();
            std::any_cast<std::deque<Event>&>(_events[std::type_index(typeid(Event))]).push_front(std::forward<Event>(event));
            _eventMutex.unlock();
        }

        /**
         * @brief Run the main loop of the registry.
         * 
         */
        void runLoop();

        /**
         * @brief Set the loop status of the registry.
         * 
         * @param status Status of the loop
         */
        void setLoop(bool status);

        /**
         * @brief Get the loop status of the registry.
         * 
         * @return true Is looping
         * @return false Is not looping
         */
        bool isLooping() const;
    private:
        //Entity
        std::vector<std::size_t> _freeIndices; /**< A queue of free entity indices. */
        std::size_t _entity_count; /**< The number of entities in the registry. */
        
        //Component
        std::unordered_map<std::type_index, std::any> _components; /**< A map of component type indices to SparseArrays containing components of that type. */
        std::vector<std::function<void(Registry &, Entity const &)>> _functions; /**< A vector of functions to be called when a component of a registered type is added to an entity. */
        
        //System
        std::vector<std::shared_ptr<ISystem>> _systems; /**< A vector of functions representing the systems in the registry. */

        //Event
        EventsWorker _eventsWorker; /**< The event worker of the registry. */
        std::mutex _eventMutex; /**< A mutex to lock the event queues. */
        std::unordered_map<std::type_index, std::any> _events; /**< A map of event type indices to queues containing events of that type. */

        //Settings
        bool _isLooping; /**< The loop status of the registry. */
};
