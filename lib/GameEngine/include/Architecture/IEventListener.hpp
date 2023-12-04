/*
** EPITECH PROJECT, 2023
** EntityComponentSystem
** File description:
** IEventListener
*/

#pragma once

class Registry;

/**
 * @brief Interface of the event listeners of the ECS
 * 
 */
class IEventListener {
    public:
        virtual ~IEventListener() = default;
        virtual void handleEvents(Registry &registry) = 0;
};
