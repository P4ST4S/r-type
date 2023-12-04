/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** StartFrameSystem
*/

#include "StartFrameSystem.hpp"

StartFrameSystem::StartFrameSystem(std::shared_ptr<RGraphic::IGraphic> lib) : _lib(lib), _clock()
{
}

StartFrameSystem::~StartFrameSystem()
{
}

void StartFrameSystem::operator()(Registry &registry)
{
    _lib->startFrame();
}
