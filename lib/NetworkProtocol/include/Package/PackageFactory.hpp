/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** PackageFactory
*/

#pragma once

#include "Package.hpp"
#include "BodyFactory.hpp"

class PackageFactory {
public:
    PackageFactory();
    ~PackageFactory();

    /**
     ** @brief Create a package class that you should fill with data before sending it.
     ** If the package type does not exist, it will throw an exception.
     **
     ** @param type
     ** @return Package
     */
    Package createPackage(enum CommandType_u type);
    /**
     ** @brief Let you know if the package type exist.
     **
     ** @param type
     ** @return true
     ** @return false
     */
    bool packageExist(enum CommandType_u type);
    /**
     ** @brief Get the BodyFactory
     **
     ** @return BodyFactory&
     */
    BodyFactory &getBodyFactory(void);
protected:
    BodyFactory _bodyFactory;
private:
};
