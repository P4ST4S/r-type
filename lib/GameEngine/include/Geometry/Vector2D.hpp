/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** Vector2D
*/

#pragma once

#include <string>
#include <ostream>

/**
 * @brief Create a vector of 2 dimensions
 *
 * @tparam T types of the position x and y of the vector
 */
template <typename T>
class Vector2D {
    public:
        /**
         * @brief Construct a new Vector of 2 dimensions with x and y at 0
         *
         */
        Vector2D() : _x(0), _y(0) {};

        /**
         * @brief Construct a new Vector of 2 dimensions with x and y
         *
         * @param x position x of the vector
         * @param y position y of the vector
         */
        Vector2D(T x, T y) : _x(x), _y(y) {};
        ~Vector2D() = default;

        /**
         * @brief add the vector rhs to the current vector
         *
         * @param rhs vector to add with this
         * @return Vector2D& the result the reference to this
         */
        Vector2D &operator+=(const Vector2D &rhs)
        {
            _x += rhs._x;
            _y += rhs._y;
            return *this;
        }

        /**
         * @brief add the vector rhs to the current vector
         *
         * @param rhs vector to add with this
         * @return Vector2D& the result of the addition
         */
        Vector2D operator+(const Vector2D &rhs) const
        {
            Vector2D tmp(*this);
            tmp += rhs;
            return tmp;
        }

        /**
         * @brief multiply the vector rhs to the current vector
         *
         * @param rhs vector to multiply with this
         * @return Vector2D& the result the reference to this
         */
        Vector2D &operator*=(const Vector2D &rhs)
        {
            _x *= rhs._x;
            _y *= rhs._y;
            return *this;
        }

        /**
         * @brief multiply the coefficient to the current vector
         *
         * @param coef coefficient to multiply with this
         * @return Vector2D& result the reference to this
         */
        Vector2D &operator*=(double coef)
        {
            _x *= coef;
            _y *= coef;
            return *this;
        }

        /**
         * @brief mutliply the vector rhs to the current vector
         *
         * @param rhs vector to multply with this
         * @return Vector2D& the result of the multiplication
         */
        Vector2D operator*(const Vector2D &rhs) const
        {
            Vector2D tmp(*this);
            tmp *= rhs;
            return tmp;
        }

        /**
         * @brief multiply the coefficient to the current vector
         *
         * @param coef coefficient to multiply with this
         * @return Vector2D& the result of the multiplication
         */
        Vector2D operator*(double coef) const
        {
            Vector2D tmp(*this);
            tmp *= coef;
            return tmp;
        }

        T _x; // position x of the vector
        T _y; // position y of the vector
};

template <typename T>
std::ostream &operator<<(std::ostream &os, const Vector2D<T> &vect)
{
    return (os << "{" << vect._x << ", " << vect._y << "}");
}
