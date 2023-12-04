/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** Package
*/

#include "Package.hpp"

Package::Package() :
_body(new EmptyBody)
{
}

Package::Package(BodyFactory &bodyFactory, const Header &header)
{
    IBody *body = bodyFactory.createBody(header.get()._commandType);

    this->_header.deserialize(header.serialize());
    this->setBody(body);
}

Package::Package(BodyFactory &bodyFactory, CommandType_t commandType)
{
    this->setBody(bodyFactory.createBody(commandType));
    this->getHeader().get()._commandType = commandType;
}

Package::Package(const Header &header, IBody *body)
{
    this->_header = header;
    this->setBody(body);
}

Package::Package(BodyFactory &bodyFactory, const std::deque<std::uint8_t> &data)
{
    this->deserialize(bodyFactory, data);
}

Package::Package(const Package &package)
{
    this->copy(package);
}

Package::Package(Package &&package)
{
    this->_header = std::move(package._header);
    this->_body = std::move(package._body);
}

Package::~Package()
{
}

void Package::copy(const Package &package) noexcept
{
    this->_header = package._header;
    this->_body.reset(package._body->clone());
}

Header &Package::getHeader(void) noexcept
{
    return (this->_header);
}

const Header &Package::getHeader(void) const noexcept
{
    return (this->_header);
}

void Package::setBody(IBody *body) noexcept
{
    if (body != nullptr)
        this->_body.reset(body);
    else
        this->_body.reset(new EmptyBody);
    this->updateBodySize();
}

void Package::updateBodySize(void) noexcept
{
    this->_header.get()._bodySize = this->_body->size();
}

std::deque<uint8_t> Package::serialize(void) const noexcept
{
    Header headerCpy = this->_header;
    size_t tmpSize;
    std::deque<uint8_t> bodyBytes = this->_body->serialize();
    std::deque<uint8_t> data;

    headerCpy.get()._bodySize = bodyBytes.size();

    // std::cout << "Sent" << std::endl;

    data = headerCpy.serialize();
    data.insert(data.end(), bodyBytes.begin(), bodyBytes.end());

    return (std::move(data));
}

bool Package::deserialize(BodyFactory &bodyFactory, const std::deque<std::uint8_t> &data)
{
    Header tmpHeader;
    std::deque<uint8_t> bodyData = data;

    this->_header.deserialize(data);
    if (data.size() < tmpHeader.get()._bodySize + HEADER_SIZE)
        throw std::runtime_error("Invalid body size");

    this->setBody(bodyFactory.createBody(this->_header.get()._commandType));
    for (size_t i = 0; i < HEADER_SIZE; i++)
        bodyData.pop_back();
    this->_body->deserialize(bodyData);
    return (true);
}

size_t Package::size(void) const noexcept
{
    return (this->_header.size() + this->_body->size());
}
