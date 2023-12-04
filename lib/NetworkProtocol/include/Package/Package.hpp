/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** Package
*/

#pragma once

#include "Body.hpp"
#include "Header.hpp"
#include "BodyFactory.hpp"
#include "EmptyBody.hpp"

#include <memory>
#include <cstring>
#include <vector>
#include <deque>

class Package {
public:
    /**
     ** @brief Create an empty package
     **
     */
    Package();

    /**
     ** @brief Construct a new Package with a header set and a body initialized.
     **
     ** @param bodyFactory
     ** @param header
     ** @param commandType
     */
    Package(BodyFactory &bodyFactory, CommandType_t commandType);

    /**
     ** @brief Construct a new Package with a set header,
     ** and a corresponding not initialized body.
     ** The body will be an Body<Empty> if the header commandType is not valid.
     **
     ** @param header
     */
    Package(BodyFactory &bodyFactory, const Header &header);

    /**
     ** @brief Construct a new Package with a header and a body directly set.
     ** The given body pointer will be the one used in the package.
     ** When this class is destructed or the body replaced,
     ** this given pointer will be freed.
     ** The header size is updated to the body size.
     **
     ** @param header
     ** @param body
     */
    Package(const Header &header, IBody *body);

    /**
     ** @brief Construct a new Package from byte,
     ** however the package must be complete.
     **
     ** @param data
     */
    Package(BodyFactory &bodyFactory, const std::deque<std::uint8_t> &data);

    Package(const Package &package);

    Package(Package &&package);

    /**
     ** @brief Destroy the Package class
     **
     */
    ~Package();

    Package operator=(const Package &other) {
        if (this != &other) {
            this->copy(other);
        }
        return (*this);
    }

    Package &operator=(Package &&other) {
        if (this != &other) {
            this->_header = std::move(other._header);
            this->_body = std::move(other._body);
        }
        return (*this);
    }

    /**
     ** @brief Copy a package into this one.
     **
     ** @param other
     */
    void copy(const Package& other) noexcept;

    /**
     ** @brief Let you acces to the body factory in this class.
     **
     ** @return BodyFactory&
     */
    BodyFactory &getBodyFactory(void) const noexcept;

    /**
     ** @brief Let you access and modify the header class in this class.
     **
     ** @return Header&&
     */
    Header &getHeader(void) noexcept;

    /**
     ** @brief Let you access to the header class in this class.
     **
     ** @return Header&
     */
    const Header &getHeader(void) const noexcept;

    /**
     ** @brief Let you access to the body class in this class.
     ** If no body has been set it will return an Body<Empty>
     **
     ** @return IBody&
     */
    template <typename T>
    T &getBody(void) noexcept
    {
        this->updateBodySize();
        return (*(reinterpret_cast<T*>(this->_body.get())));
    }

    /**
     ** @brief Let you access to the body class in this class.
     ** If no body has been set it will return an EmptyBody
     **
     ** @return IBody&
     */
    template <typename T>
    const T &getBody(void) const noexcept
    {
        return (*(reinterpret_cast<T*>(this->_body.get())));
    }

    /**
     ** @brief Let you set or change the body.
     ** The given body pointer will be the one used in the package.
     ** When this class is destructed or the body replaced,
     ** this given pointer will be freed.
     ** The header size is updated.
     **
     ** @param body
     */
    void setBody(IBody *body) noexcept;

    /**
     ** @brief Ensure the header body size member is up to date.
     **
     */
    void updateBodySize(void) noexcept;

    /**
     ** @brief Transform the whole package into a byte array.
     **
     ** @return std::vector<uint8_t>&
     */
    std::deque<uint8_t> serialize(void) const noexcept;

    /**
     ** @brief
     **
     ** @param data
     ** @return true
     ** @return false
     */
    bool deserialize(BodyFactory &bodyFactory, const std::deque<uint8_t> &data);

    /**
     ** @brief Return the size of the package in bytes.
     **
     ** @return size_t
     */
    size_t size(void) const noexcept;

private:

    Header _header;
    std::unique_ptr<IBody> _body;
};
