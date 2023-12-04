/*
** EPITECH PROJECT, 2023
** InputEventListeber
** File description:
** InputEventListener
*/

#include "InputEventListener.hpp"

InputEvent::InputEvent(RGraphic::RInputs_t inputs) : _inputs(inputs)
{
};

RGraphic::RInputs_t InputEvent::getInputs() const
{
    return (_inputs);
};

InputEventListener::InputEventListener(std::shared_ptr<RGraphic::IGraphic> &graphic) : _graphic(graphic), _clock()
{
};

void InputEventListener::handleEvents(Registry &registry)
{
    RGraphic::RInputs_t inputs = _graphic->getInputs();

    _clock.cap(200);
    if (inputs.down || inputs.up || inputs.left || inputs.right || inputs.pause || inputs.shoot){
        registry.addEvent(InputEvent(inputs));
    }
};
