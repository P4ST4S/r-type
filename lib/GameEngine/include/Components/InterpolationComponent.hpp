/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** InterpolationComponent
*/

#pragma once

#include "IComponent.hpp"
#include "Vector2D.hpp"
#include "BodyStruct.hpp"

#include <chrono>

class InterpolationComponent : public IComponent {
public:
    InterpolationComponent();
    InterpolationComponent(Vector2D<double> position);
    InterpolationComponent(Vector2D<double> position, Vector2D<double> vector);
    ~InterpolationComponent();

    /**
     ** @brief Time when the position is not influenced by the vector.
     **
     ** @param startTime
     */
    void setStartTime(const std::chrono::duration<double> &startTime);
    /**
     ** @brief The time that should elapsed between the start and the end of the movement
     **
     ** @param duration
     */
    void setDuration(double duration);

    /**
     ** @brief Set the position useful when teleporting entity.
     **
     ** @param vector
     */
    void setPosition(const Vector2D<double> &vector);

    /**
     ** @brief Set the position useful when teleporting entity.
     **
     ** @param x
     ** @param y
     */
    void setPosition(double x, double y);

    /**
     ** @brief Set the movement that'll be done in the set duration.
     **
     ** @param vector
     */
    void move(const Vector2D<double> &vector);

    /**
     ** @brief Set the movement that'll be done in the set duration.
     **
     ** @param x
     ** @param y
     */
    void move(double x, double y);

    /**
     ** @brief Should be called before using move method to update position.
     **
     */
    void applyMove();

    Vector2D<double> getPosition(void) const;
    Vector2D<double> getFinalPosition(void) const;
    Vector2D<double> getVector(void) const;
    double getInterpolationCoefficient(const std::chrono::_V2::system_clock::time_point &time) const;
    Vector2D<double> getInterpoledPosition(const std::chrono::_V2::system_clock::time_point &time) const;

    /**
     ** @brief Generate a MoveEntitiesDyn_t body from the current position and vector.
     **
     ** @param entityId
     ** @param body
     */
    void getMoveEntityBody(size_t entityId, MoveEntitiesDyn_t &body) const;

    void setMoveEntityBody(MoveEntitiesDyn_t &body);

    void replacePosition(void);
protected:

    std::chrono::duration<double> _startTime;
    std::chrono::duration<double> _duration = std::chrono::duration<double>(1.0 / 10.0);
    Vector2D<double> _position;
    Vector2D<double> _vector;
private:
};

/**
 * @brief destroy the component InterpolationComponent of an entity
 *
 * @param registry Registry of the game, used to destroy the component
 * @param entity Entity which will lose the component
 */
void destroyInterpolation(Registry &registry, Entity const &entity);
