/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** CheckboxSystem
*/

#include "CheckboxSystem.hpp"

CheckboxSystem::CheckboxSystem(std::shared_ptr<RGraphic::IGraphic> lib) : _lib(lib)
{
}

CheckboxSystem::~CheckboxSystem()
{
}

void CheckboxSystem::operator()(Registry &registry, SparseArray<PositionComponent> &positionArray, SparseArray<CheckboxComponent> &CheckboxArray)
{
    Zipper<SparseArray<PositionComponent>, SparseArray<CheckboxComponent>> zipper(positionArray, CheckboxArray);

    for (auto it = zipper.begin(); it != zipper.end(); it++) {
        CheckboxComponent &Checkbox = std::get<CheckboxComponent&>(*it);
        Vector2D<double> size = Checkbox.getRectangle();
        Vector2D<double> position = std::get<PositionComponent&>(*it).getPosition();
        _lib->drawCheckbox({position._x, position._y, size._x, size._y}, Checkbox.getText().c_str(), Checkbox.getChecked());
    }
}
