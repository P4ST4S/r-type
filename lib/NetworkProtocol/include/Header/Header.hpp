/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** Header
*/

#pragma once

#include "CommandTypes.hpp"
#include "BodyFactory.hpp"
#include "CpnUtils.hpp"

#include <string>
#include <cstring>
#include <cstdint>
#include <vector>
#include <stdexcept>
#include <deque>

#define HEADER_SIZE sizeof(struct HeaderData_s)
#define MAGIC_NUMBER "RTYP"
#define MAGIC_NUMBER_SIZE 4

typedef struct HeaderData_s {
    const uint32_t _magicNumber;   ///< The magic number for package identification.
    uint32_t _bodySize;         ///< The size of the package body.
    enum CommandType_u _commandType;      ///< The type of command associated with the package.

    HeaderData_s() :
        _magicNumber(*(reinterpret_cast<const uint32_t*>(MAGIC_NUMBER))),
        _bodySize(0), _commandType(UNKNOWN) {}
} HeaderData_t;

/**
 * @class Header
 * @brief Represents the header of a protocol package.
 */
class Header {
public:
    /**
     * @brief Default constructor.
     */
    Header();

    /**
     * @brief Constructor to initialize the header.
     *
     * @param commandType The type of command associated with the package.
     * @param senderId The unique identifier for the package.
     */
    Header(BodyFactory &bodyFactory, enum CommandType_u commandType);

    /**
     * @brief Constructor to initialize the header.
     *
     * @param commandType The type of command associated with the package.
     * @param bodySize The size of the package body.
     * @param senderId The unique identifier for the package.
     */
    Header(enum CommandType_u commandType, uint32_t bodySize);

    Header(const std::deque<uint8_t> &data);

    Header(const Header &header);

    Header operator=(const Header& other) {
        if (this != &other) {
            this->copy(other);
        }
        return (*this);
    }

    /**
     * @brief Destructor.
     */
    ~Header();

    void copy(const Header &header);

    /**
     ** @brief Let you access to header members.
     **
     ** @return HeaderData_t&
     */
    HeaderData_t &get(void);

    /**
     ** @brief Let you access to header members but you can't modify them.
     **
     ** @return HeaderData_t&
     */
    const HeaderData_t &get(void) const;

    /**
     * @brief Serialize the header into a byte vector.
     * @return A byte vector containing the serialized header.
     */
    std::deque<uint8_t> serialize() const;

    /**
     * @brief Deserialize a byte vector into a Header object.
     * @param data The byte vector containing the serialized header.
     * @return A Header object representing the deserialized header.
     * @throw std::runtime_error if the data vector is too small for deserialization.
     */
    void deserialize(const std::deque<uint8_t> &data);

    /**
     ** @brief Get the header size in bytes.
     **
     ** @return size_t
     */
    size_t size(void) const;


private:
    HeaderData_t _header;
    const size_t _size = HEADER_SIZE;
};

std::ostream &operator<<(std::ostream &os, const Header &header);
