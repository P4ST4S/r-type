/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** Body
*/

#pragma once

#include "CommandTypes.hpp"
#include "CpnUtils.hpp"

#include <cstdint>
#include <vector>
#include <deque>
#include <stdexcept>

/**
 * @brief Interface base class for message body types.
 *
 * The `IBody` class serves as an interface base class for various message body types.
 * It defines two pure virtual functions: `serialize` for serializing the body data
 * into a byte vector, and `deserialize` for deserializing data from a byte vector into
 * the body structure.
 *
 * Subclasses of `Body` should implement these virtual functions to provide specific
 * serialization and deserialization behavior for their respective message body types.
 *
 * @note When implementing a derived class, ensure that the `deserialize` method can handle
 * the byte vector of the exact size expected for that body type.
 */
class IBody {
public:
    /**
     * @brief Destructor for the `IBody` class.
     */
    virtual ~IBody() = default;

    /**
     * @brief Serialize the message body to a byte vector.
     *
     * This function should be implemented by derived classes to convert the message body
     * data into a byte vector suitable for transmission over a network or storage.
     *
     * @return A byte vector containing the serialized message body data.
     */
    virtual std::deque<std::uint8_t> serialize() const noexcept = 0;

    /**
     * @brief Deserialize the message body from a byte vector.
     *
     * This function should be implemented by derived classes to extract and populate
     * the message body data from a given byte vector.
     *
     * @param data The byte vector containing the serialized message body data.
     * @throws std::runtime_error if the deserialization process fails.
     */
    virtual void deserialize(const std::deque<std::uint8_t> &data) = 0;

    /**
     ** @brief Get the size of the body
     **
     ** @return size_t
     */
    virtual size_t size(void) const noexcept = 0;

    /**
     ** @brief Create a copy of this Body class.
     **
     ** @return IBody*
     */
    virtual IBody *clone(void) const noexcept = 0;

    /**
     ** @brief This method is required for the body factory.
     ** Create a new Body class unrelated from this one except for it's type.
     **
     ** @return IBody*
     */
    virtual IBody *cloneNew(void) const noexcept = 0;
};
