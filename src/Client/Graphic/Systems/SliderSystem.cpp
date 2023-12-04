/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** SliderSystem
*/

#include "SliderSystem.hpp"

SliderSystem::SliderSystem(std::shared_ptr<RGraphic::IGraphic> lib) : _lib(lib)
{
}

SliderSystem::~SliderSystem()
{
}

void SliderSystem::operator()(Registry &registry, SparseArray<PositionComponent> &positionArray, SparseArray<SliderComponent> &SliderArray)
{
    Zipper<SparseArray<PositionComponent>, SparseArray<SliderComponent>> zipper(positionArray, SliderArray);

    for (auto it = zipper.begin(); it != zipper.end(); it++) {
        SliderComponent &slider = std::get<SliderComponent&>(*it);
        Vector2D<double> size = slider.getRectangle();
        Vector2D<double> position = std::get<PositionComponent&>(*it).getPosition();
        _lib->drawSlider({position._x, position._y, size._x, size._y}, slider.getTextLeft(), slider.getTextRight(), slider.getValue());
    }
}
