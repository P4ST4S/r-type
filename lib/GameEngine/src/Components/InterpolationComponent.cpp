/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** InterpolationComponent
*/

#include "InterpolationComponent.hpp"

InterpolationComponent::InterpolationComponent()
{
    this->_position._x = 0;
    this->_position._y = 0;
    this->_vector._x = 0;
    this->_vector._y = 0;
}


InterpolationComponent::InterpolationComponent(Vector2D<double> position) : _position(position)
{
}

InterpolationComponent::InterpolationComponent(Vector2D<double> position, Vector2D<double> vector) : _position(position), _vector(vector)
{
}

InterpolationComponent::~InterpolationComponent()
{
}

void InterpolationComponent::setStartTime(const std::chrono::duration<double> &startTime)
{
    this->_startTime = startTime;
}

void InterpolationComponent::setDuration(double duration)
{
    this->_duration = std::chrono::duration<double>(duration);
}

void InterpolationComponent::setPosition(const Vector2D<double> &vector)
{
    this->_position = vector;
}

void InterpolationComponent::setPosition(double x, double y)
{
    this->_position._x = x;
    this->_position._y = y;
    // this->replacePosition();
}

void InterpolationComponent::move(const Vector2D<double> &vector)
{
    this->_vector = vector;
}

void InterpolationComponent::move(double x, double y)
{
    this->_vector._x = x;
    this->_vector._y = y;
}

void InterpolationComponent::applyMove()
{
    this->_position += this->_vector;
    this->_vector._x = 0;
    this->_vector._y = 0;
    // this->replacePosition();
}

Vector2D<double> InterpolationComponent::getPosition(void) const
{
    return this->_position;
}


Vector2D<double> InterpolationComponent::getFinalPosition(void) const
{
    return (this->_position + this->_vector);
}

Vector2D<double> InterpolationComponent::getVector(void) const
{
    return this->_vector;
}

double InterpolationComponent::getInterpolationCoefficient(const std::chrono::_V2::system_clock::time_point &time) const
{
    std::chrono::duration<double> elapsed = time.time_since_epoch() - this->_startTime;

    return (elapsed.count() / this->_duration.count());
}

Vector2D<double> InterpolationComponent::getInterpoledPosition(const std::chrono::_V2::system_clock::time_point &time) const
{
    return this->_position + (this->_vector * this->getInterpolationCoefficient(time));
}

void InterpolationComponent::getMoveEntityBody(size_t entityId, MoveEntitiesDyn_t &body) const
{
    body._id = static_cast<int16_t>(entityId);
    body._x = static_cast<int16_t>(this->_position._x);
    body._y = static_cast<int16_t>(this->_position._y);
    body._vx = static_cast<int16_t>(this->_vector._x);
    body._vy = static_cast<int16_t>(this->_vector._y);
}

void InterpolationComponent::setMoveEntityBody(MoveEntitiesDyn_t &body)
{
    this->_vector._x = body._vx;
    this->_vector._y = body._vy;
    this->_position._x = body._x;
    this->_position._y = body._y;
    // this->replacePosition();
}

void InterpolationComponent::replacePosition(void)
{
    if (_position._x < 0)
        _position._x = 0;
    if (_position._x > INTERNAL_WIDTH)
        _position._x = INTERNAL_WIDTH;
    if (_position._y < 0)
        _position._y = 0;
    if (_position._y > INTERNAL_HEIGHT)
        _position._y = INTERNAL_HEIGHT;
}


void destroyInterpolation(Registry &registry, Entity const &entity)
{
    SparseArray<InterpolationComponent> &interpolationArray = registry.getComponents<InterpolationComponent>();

    if (interpolationArray.size() > entity)
        interpolationArray[entity].reset();
}
