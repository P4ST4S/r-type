/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** InputBoxSystem
*/

#include "InputBoxSystem.hpp"

InputBoxSystem::InputBoxSystem(std::shared_ptr<RGraphic::IGraphic> lib) : _lib(lib)
{
}

InputBoxSystem::~InputBoxSystem()
{
}

void InputBoxSystem::operator()(Registry &registry, SparseArray<PositionComponent> &positionArray, SparseArray<InputBoxComponent> &inputBoxComponent)
{
    Zipper<SparseArray<PositionComponent>, SparseArray<InputBoxComponent>> zipper(positionArray, inputBoxComponent);

    for (auto it = zipper.begin(); it != zipper.end(); it++) {
        InputBoxComponent &inputBox = std::get<InputBoxComponent&>(*it);
        Vector2D<double> rect = inputBox.getRectangle();
        Vector2D<double> position = std::get<PositionComponent&>(*it).getPosition();
        _lib->drawInputBox({position._x, position._y, rect._x, rect._y}, inputBox.getText(), inputBox.getTextSize() + 1, inputBox.getEditMode());
    }
}
