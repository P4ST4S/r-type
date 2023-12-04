/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** DynamicBody
*/

#pragma once

#include "IBody.hpp"
#include "Body.hpp"
#include "DynamicBody.hpp"

#include <vector>

template <typename StaticPart, typename DynamicPart>
class HybridBody : public IBody
{
public:
    HybridBody()
    {
    }
    HybridBody(const std::vector<std::uint8_t> &data)
    {
        this->deserialize(data);
    }
    HybridBody(const Body<StaticPart> &staticPart, const DynamicBody<DynamicPart> &dynamicPart)
    {
        this->_staticBody.set(staticPart);
        this->_dynamicBody.set(dynamicPart);
    }
    HybridBody(const HybridBody &other)
    {
        this->_body.first = other._body.first;
        this->_body.second = other._body.second;
    }
    HybridBody(HybridBody &&other)
    {
        this->_body.first = std::move(other._body.first);
        this->_body.second = std::move(other._body.second);
    }
    ~HybridBody(){};

    HybridBody operator=(const HybridBody& other) {
        if (this != &other) {
            this->_body.first = other._body.first;
            this->_body.second = other._body.second;
        }
        return (*this);
    }

    HybridBody& operator=(const HybridBody&& other) {
        if (this != &other) {
            this->_body.first = std::move(other._body.first);
            this->_body.second = std::move(other._body.second);
        }
        return (*this);
    }

    void clear(void) noexcept
    {
        this->_body.clear();
    }

    std::pair<Body<StaticPart>, DynamicBody<DynamicPart>> &get() noexcept
    {
        return (this->_body);
    }

    const std::pair<Body<StaticPart>, DynamicBody<DynamicPart>> &get() const noexcept
    {
        return (this->_body);
    }

    std::deque<std::uint8_t> serialize(void) const noexcept
    {
        std::deque<std::uint8_t> data = this->_body.first.serialize();
        std::deque<std::uint8_t> data2 = this->_body.second.serialize();

        data.insert(data.end(), data2.begin(), data2.end());
        return (std::move(data));
    }

    void deserialize(const std::deque<std::uint8_t> &data)
    {
        size_t size;
        std::deque<std::uint8_t> tmpData;
        // if (data.size() < this->_body.first.size())
        //     throw std::runtime_error("Invalid static body size for deserialization");
        // if ((data.size() - this->_body.first.size()) % sizeof(T) != 0)
        //     throw std::runtime_error("Invalid dynamic body size for deserialization");
        this->_body.first.deserialize(data);
        tmpData.insert(tmpData.end(), data.begin() + this->_body.first.size(), data.end());
        this->_body.second.deserialize(tmpData);
    }

    size_t size() const noexcept
    {
        return (this->_body.first.size() + this->_body.second.size());
    }

    HybridBody<StaticPart, DynamicPart> *clone(void) const noexcept
    {
        return (new HybridBody<StaticPart, DynamicPart>(*this));
    }

    HybridBody<StaticPart, DynamicPart> *cloneNew(void) const noexcept
    {
        return (new HybridBody<StaticPart, DynamicPart>);
    }

protected:
    std::pair<Body<StaticPart>, DynamicBody<DynamicPart>> _body;
private:
};
