/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** EmptyBody
*/

#include "EmptyBody.hpp"

EmptyBody::EmptyBody()
{
}

EmptyBody::EmptyBody(const EmptyBody& other)
{
}

EmptyBody::EmptyBody(EmptyBody&& other)
{
}

EmptyBody::~EmptyBody()
{
}

std::deque<std::uint8_t> EmptyBody::serialize(void) const noexcept
{
    return (std::move(std::deque<std::uint8_t>()));
}

void EmptyBody::deserialize(const std::deque<std::uint8_t> &data)
{
}

size_t EmptyBody::size(void) const noexcept
{
    return (this->_size);
}

EmptyBody *EmptyBody::clone(void) const noexcept
{
    return (new EmptyBody);
}

EmptyBody *EmptyBody::cloneNew(void) const noexcept
{
    return (new EmptyBody);
}
