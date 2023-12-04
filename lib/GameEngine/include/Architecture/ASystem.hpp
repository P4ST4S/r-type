/*
** EPITECH PROJECT, 2023
** EntityComponentSystem
** File description:
** ASystem
*/

#pragma once

#include "ISystem.hpp"

class Registry;

class ASystem : public ISystem
{
    public:
        virtual void operator()(Registry &registry) override;
};
