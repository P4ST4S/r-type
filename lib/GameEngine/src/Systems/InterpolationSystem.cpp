/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** InterpolationSystem
*/

#include "InterpolationSystem.hpp"

InterpolationSystem::InterpolationSystem(double &screenWith, double &screenHeight) :
_screenWidth(screenWith), _screenHeight(screenHeight)
{
}

InterpolationSystem::~InterpolationSystem()
{
}

void InterpolationSystem::operator()(Registry &registry,
                SparseArray<InterpolationComponent> &interpolationComponents,
                SparseArray<PositionComponent> &positionComponents)
{
    double coef = (static_cast<double>(this->_screenWidth) / INTERNAL_WIDTH);
    std::chrono::_V2::system_clock::time_point now = std::chrono::system_clock::now();
    for (std::size_t i = 0; i < interpolationComponents.size() && i < positionComponents.size(); ++i){
        if (interpolationComponents[i].has_value() && positionComponents[i].has_value()) {
            InterpolationComponent &interpolation = *interpolationComponents[i];
            PositionComponent &position = (*positionComponents[i]);

            // std::cout << this->_screenWidth << " " << INTERNAL_WIDTH << " " << coef << " " << (interpolation.getInterpoledPosition(now) * coef) << std::endl;
            position.setPosition(interpolation.getInterpoledPosition(now) * coef);
        }
    }
}
