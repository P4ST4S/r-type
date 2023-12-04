/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** ButtonSystem
*/

#include "ButtonSystem.hpp"

ButtonSystem::ButtonSystem(std::shared_ptr<RGraphic::IGraphic> lib) : _lib(lib)
{
}

ButtonSystem::~ButtonSystem()
{
}

void ButtonSystem::operator()(Registry &registry, SparseArray<PositionComponent> &positionArray, SparseArray<ButtonComponent> &buttonArray)
{
    Zipper<SparseArray<PositionComponent>, SparseArray<ButtonComponent>> zipper(positionArray, buttonArray);

    for (auto it = zipper.begin(); it != zipper.end(); it++) {
        ButtonComponent &button = std::get<ButtonComponent&>(*it);
        Vector2D<double> size = button.getRectangle();
        Vector2D<double> position = std::get<PositionComponent&>(*it).getPosition();
        if (button.getShown())
            _lib->drawButton({position._x, position._y, size._x, size._y}, button.getText().c_str(), button.getShown(), button.getOnClick());
    }
}
