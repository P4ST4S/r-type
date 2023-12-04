/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** Header
*/

#include "Header.hpp"

Header::Header()
{
}

Header::Header(BodyFactory &bodyFactory, enum CommandType_u commandType)
{
    this->_header._bodySize = bodyFactory.getBodySize(commandType);
    this->_header._commandType = commandType;
}

Header::Header(enum CommandType_u commandType, uint32_t bodySize)
{
    this->_header._bodySize = bodySize;
    this->_header._commandType = commandType;
}

Header::Header(const std::deque<uint8_t> &data)
{
    this->deserialize(data);
}

Header::Header(const Header &header)
{
    this->copy(header);
}

Header::~Header() {}

void Header::copy(const Header &header)
{
    this->deserialize(header.serialize());
}

HeaderData_t &Header::get(void)
{
    return (this->_header);
}

const HeaderData_t &Header::get(void) const
{
    return (this->_header);
}

std::deque<uint8_t> Header::serialize() const
{
    std::deque<uint8_t> data(this->_size, 0);

    memcpy(DEQUE_DATA(data), &(this->_header), this->_size);

    return (std::move(data));
}

void Header::deserialize(const std::deque<uint8_t> &data)
{
    if (data.size() < HEADER_SIZE)
        throw std::runtime_error("Invalid body size for deserialization");
    memcpy(&(this->_header), DEQUE_DATA(data), this->_size);
}

size_t Header::size(void) const
{
    return (this->_size);
}

std::ostream &operator<<(std::ostream &os, const Header &header)
{
    const uint8_t *magicNumber = reinterpret_cast<const uint8_t*>(&header.get()._magicNumber);
    os << "Magic Number: ";
    for (size_t i = 0; i < MAGIC_NUMBER_SIZE; ++i)
        os << magicNumber[i];
    os << "\n";
    os << "Body Size   : " << header.get()._bodySize << "\n";
    os << "Command Type: " << header.get()._commandType << "\n";
    return (os);
}
