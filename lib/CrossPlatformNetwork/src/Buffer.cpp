/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** CPNBuffer
*/

#include "Buffer.hpp"

cpn::Buffer::Buffer()
{
}

cpn::Buffer::Buffer(const void *data, size_t size)
{
    this->writeIn(data, size);
}

cpn::Buffer::Buffer(const std::deque<uint8_t> &data, size_t size)
{
    this->writeIn(data, size);
}

cpn::Buffer::Buffer(const std::deque<uint8_t> &data)
{
    this->writeIn(data);
}

cpn::Buffer::Buffer(const Buffer &other)
{
    this->copy(other);
}

cpn::Buffer::Buffer(Buffer &&other)
{
    this->_buffer = std::move(other._buffer);
}

cpn::Buffer::~Buffer()
{
}

void cpn::Buffer::copy(const Buffer &other) noexcept
{
    this->_buffer = other._buffer;
}

size_t cpn::Buffer::writeIn(const void *data, size_t size) noexcept
{
    size_t prevSize = this->_buffer.size();

    this->_buffer.resize(prevSize + size);
    memcpy(DEQUE_DATA(this->_buffer) + prevSize, data, size);
    return (size);
}

size_t cpn::Buffer::writeIn(const std::deque<uint8_t> &data, size_t size) noexcept
{
    size_t i = 0;
    for (; i < size && i < data.size(); ++i)
        this->_buffer.push_back(data[i]);

    return (i);
}

size_t cpn::Buffer::writeIn(const std::deque<uint8_t> &data) noexcept
{
    this->_buffer.insert(this->_buffer.end(), data.begin(), data.end());
    return (data.size());
}


std::deque<uint8_t> cpn::Buffer::readIn(size_t size) noexcept
{
    std::deque<uint8_t> data;

    if (size >= this->_buffer.size())
        return (this->readIn());
    while (size > 0 && !this->_buffer.empty()) {
        data.push_back(this->_buffer.front());
        this->_buffer.pop_front();
        --size;
    }
    return (std::move(data));
}

std::deque<uint8_t> cpn::Buffer::readIn(void) noexcept
{
    std::deque<uint8_t> data = std::move(this->_buffer);

    this->clear();
    return (std::move(data));
}

void cpn::Buffer::clear(void) noexcept
{
    this->_buffer.resize(0);
}

size_t cpn::Buffer::size(void) const noexcept
{
    return (this->_buffer.size());
}

std::deque<uint8_t> &cpn::Buffer::get(void) noexcept
{
    return (this->_buffer);
}

const std::deque<uint8_t> &cpn::Buffer::get(void) const noexcept
{
    return (this->get());
}
