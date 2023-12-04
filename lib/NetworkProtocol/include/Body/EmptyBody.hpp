/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** EmptyBody
*/

#pragma once

#include "IBody.hpp"
#include "BodyStruct.hpp"

class EmptyBody : public IBody {
public:
    EmptyBody();
    EmptyBody(const EmptyBody& other);
    EmptyBody(EmptyBody&& other);
    ~EmptyBody();

    EmptyBody operator=(const EmptyBody& other)
    {
        return (*this);
    }

    EmptyBody &operator=(EmptyBody&& other)
    {
        return (*this);
    }

    std::deque<std::uint8_t> serialize(void) const noexcept;
    void deserialize(const std::deque<std::uint8_t> &data);
    size_t size(void) const noexcept;
    EmptyBody *clone(void) const noexcept;
    EmptyBody *cloneNew(void) const noexcept;
protected:
    const size_t _size = 0;
};
