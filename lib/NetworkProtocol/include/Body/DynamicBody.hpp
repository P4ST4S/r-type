/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** DynamicBody
*/

#pragma once

#include "IBody.hpp"

#include <vector>

template <typename T>
class DynamicBody : public IBody
{
public:
    DynamicBody()
    {
    }
    DynamicBody(const std::vector<std::uint8_t> &data)
    {
        this->deserialize(data);
    }
    DynamicBody(const T &bodyData)
    {
        this->set(bodyData);
    }
    DynamicBody(const std::vector<T> &bodyData)
    {
        this->set(bodyData);
    }
    DynamicBody(const DynamicBody &other)
    {
        this->_body = other._body;
    }
    DynamicBody(DynamicBody &&other)
    {
        this->_body = std::move(other._body);
    }
    ~DynamicBody(){};

    DynamicBody operator=(const DynamicBody& other) {
        if (this != &other) {
            this->_body = other._body;
        }
        return (*this);
    }

    DynamicBody& operator=(const DynamicBody&& other) {
        if (this != &other) {
            this->_body = std::move(other._body);
        }
        return (*this);
    }

    DynamicBody& operator+=(const T& bodyData) {
        this->_body.add(bodyData);
        return (*this);
    }

    DynamicBody& operator+=(const std::deque<T> &bodyData) {
        this->_body.add(bodyData);
        return (*this);
    }

    DynamicBody& operator+=(const DynamicBody<T> &bodyData) {
        this->_body.add(bodyData);
        return (*this);
    }

    // DynamicBody& operator+=(const std::vector<std::uint8_t> &data) {
    //     this->_body.add(data);
    //     return (*this);
    // }

    DynamicBody<T>& operator+(const DynamicBody<T>& bodyData) {
        DynamicBody<T> newBody(*this);

        newBody += bodyData;
        return (newBody);
    }

    void clear(void) noexcept
    {
        this->_body.clear();
    }

    void set(const T &bodyData) noexcept
    {
        this->clear();
        this->_body.push_back(bodyData);
    }

    void set(const std::deque<T> &bodyData) noexcept
    {
        this->_body = bodyData;
    }

    void add(const T &bodyData) noexcept
    {
        this->_body.push_back(bodyData);
    }

    void add(const std::deque<T> &bodyData) noexcept
    {
        this->_body.insert(this->_body.end(), bodyData.begin(), bodyData.end());
    }

    void add(const DynamicBody<T> &bodyData) noexcept
    {
        this->_body.insert(this->_body.end(), bodyData.get().begin(), bodyData.get().end());
    }

    std::deque<T> &get() noexcept
    {
        return (this->_body);
    }

    const std::deque<T> &get() const noexcept
    {
        return (this->_body);
    }

    std::deque<std::uint8_t> serialize(void) const noexcept
    {
        std::vector<uint8_t> tmp(this->size(), 0);
        T *tmpPtr = reinterpret_cast<T *>(tmp.data());

        for (size_t i = 0; i < this->elemCount(); ++i)
            tmpPtr[i] = this->_body[i];

        // memcpy(DEQUE_DATA(data), DEQUE_DATA(this->_body), this->size());
        return (std::move(std::deque<std::uint8_t>(tmp.begin(), tmp.end())));
    }

    void deserialize(const std::deque<std::uint8_t> &data)
    {
        std::vector<uint8_t> tmp(data.begin(), data.end());
        size_t elemCount;

        if (data.size() % sizeof(T) != 0)
            throw std::runtime_error("Invalid body size for deserialization");
        tmp.resize(data.size());
        T *body = reinterpret_cast<T *>(tmp.data());
        elemCount = data.size() / sizeof(T);
        this->_body.resize(elemCount);
        for (size_t i = 0; i < elemCount; ++i)
            this->_body[i] = body[i];
    }

    size_t size() const noexcept
    {
        return (this->_body.size() * sizeof(T));
    }

    size_t elemCount() const noexcept
    {
        return (this->_body.size());
    }

    DynamicBody<T> *clone(void) const noexcept
    {
        return (new DynamicBody<T>(*this));
    }

    DynamicBody<T> *cloneNew(void) const noexcept
    {
        return (new DynamicBody<T>);
    }

protected:
    std::deque<T> _body;
private:
};
