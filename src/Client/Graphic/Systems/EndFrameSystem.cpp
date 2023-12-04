/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** EndFrameSystem
*/

#include "EndFrameSystem.hpp"

EndFrameSystem::EndFrameSystem(std::shared_ptr<RGraphic::IGraphic> lib) : _lib(lib)
{
}

EndFrameSystem::~EndFrameSystem()
{
}

void EndFrameSystem::operator()(Registry &registry)
{
    std::deque<InputEvent> inputEvents = registry.getEvent<InputEvent>();

    if (_lib->windowShouldClose())
        registry.setLoop(false);
    _lib->endFrame();
}
