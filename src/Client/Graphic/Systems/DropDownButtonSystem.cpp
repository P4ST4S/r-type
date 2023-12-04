/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** DropDownButtonSystem
*/

#include "DropDownButtonSystem.hpp"

DropDownButtonSystem::DropDownButtonSystem(std::shared_ptr<RGraphic::IGraphic> lib) : _lib(lib)
{
}

DropDownButtonSystem::~DropDownButtonSystem()
{
}

void DropDownButtonSystem::operator()(Registry &registry, SparseArray<PositionComponent> &positionArray, SparseArray<DropDownButtonComponent> &dropDownButtonArray)
{
    Zipper<SparseArray<PositionComponent>, SparseArray<DropDownButtonComponent>> zipper(positionArray, dropDownButtonArray);

    for (auto it = zipper.begin(); it != zipper.end(); it++) {
        DropDownButtonComponent &dropDownButton = std::get<DropDownButtonComponent&>(*it);
        Vector2D<double> size = dropDownButton.getRectangle();
        Vector2D<double> position = std::get<PositionComponent&>(*it).getPosition();
        _lib->drawDropDownButton({position._x, position._y, size._x, size._y}, dropDownButton.getText(), dropDownButton.getActiveMode(), dropDownButton.getEditMode());
    }
}
