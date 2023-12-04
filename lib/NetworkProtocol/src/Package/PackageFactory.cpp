/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** PackageFactory
*/

#include "PackageFactory.hpp"

PackageFactory::PackageFactory()
{
}

PackageFactory::~PackageFactory()
{
}

Package PackageFactory::createPackage(enum CommandType_u type)
{
    if (!this->_bodyFactory.bodyExist(type))
        throw std::runtime_error("PackageFactory: Body type does not exist.");
    return (std::move(Package(this->_bodyFactory, type)));
}

bool PackageFactory::packageExist(enum CommandType_u type)
{
    return (this->_bodyFactory.bodyExist(type));
}

BodyFactory &PackageFactory::getBodyFactory(void)
{
    return (this->_bodyFactory);
}
