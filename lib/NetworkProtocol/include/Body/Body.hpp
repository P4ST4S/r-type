/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** Body
*/

#pragma once

#include "IBody.hpp"

#include <iostream>
#include <cstring>
#include <cstdint>
#include <vector>

template <typename T>
class Body : public IBody
{
public:
    Body()
    {
        memset(&(this->_body), 0, this->_size);
    }
    Body(const std::deque<std::uint8_t> &data) : Body()
    {
        this->deserialize(data);
    }
    Body(const T &bodyData) : Body()
    {
        this->set(bodyData);
    }
    Body(const Body<T> &other)
    {
        this->deserialize(other.serialize());
    }
    Body(Body<T> &&other)
    {
        this->_body = std::move(other._body);
    }
    ~Body(){};

    Body operator=(const Body &other)
    {
        this->deserialize(other.serialize());
        return (*this);
    }

    Body &operator=(Body &&other)
    {
        this->_body = std::move(other._body);
        return (*this);
    }

    /**
     ** @brief Let you overwrite the current body data by a new one.
     **
     ** @param bodyData
     */
    void set(const T &bodyData) noexcept
    {
        memcpy(&(this->_body), &bodyData, this->_size);
    }

    /**
     ** @brief Let you access to body members.
     **
     ** @return T&
     */
    T &get(void) noexcept
    {
        return (this->_body);
    }

    /**
     ** @brief Let you access to body members.
     **
     ** @return T&
     */
    const T &get(void) const noexcept
    {
        return (this->_body);
    }

    /**
     * @brief Serialize the message body to a byte vector.
     *
     * This function should be implemented by derived classes to convert the message body
     * data into a byte vector suitable for transmission over a network or storage.
     *
     * @return A byte vector containing the serialized message body data.
     */
    std::deque<std::uint8_t> serialize(void) const noexcept
    {
        std::vector<uint8_t> tmp(this->_size, 0);
        T *tmpPtr = reinterpret_cast<T *>(tmp.data());

        *tmpPtr = this->_body;
        return (std::move(std::deque<std::uint8_t>(tmp.begin(), tmp.end())));
    }

    /**
     * @brief Deserialize the message body from a byte vector.
     *
     * This function should be implemented by derived classes to extract and populate
     * the message body data from a given byte vector.
     *
     * @param data The byte vector containing the serialized message body data.
     * @throws std::runtime_error if the deserialization process fails.
     */
    void deserialize(const std::deque<std::uint8_t> &data)
    {
        std::vector<uint8_t> tmp(data.begin(), data.end());

        if (data.size() < sizeof(T))
            throw std::runtime_error("Invalid body size for deserialization");
        tmp.resize(sizeof(T));
        this->set(*(reinterpret_cast<T *>(tmp.data())));
    }

    /**
     ** @brief Get the size of the body
     **
     ** @return size_t
     */
    size_t size(void) const noexcept
    {
        return (this->_size);
    }

    Body<T> *clone(void) const noexcept
    {
        return (new Body<T>(*this));
    }

    Body<T> *cloneNew(void) const noexcept
    {
        return (new Body<T>);
    }

protected:
    T _body;
    const size_t _size = sizeof(T);

private:
};
