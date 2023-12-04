/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** BodyFactory
*/

#pragma once

#include "BodyStruct.hpp"
#include "EmptyBody.hpp"
#include "Body.hpp"
#include "DynamicBody.hpp"
#include "HybridBody.hpp"
#include "CommandTypes.hpp"

#include <iostream>
#include <vector>
#include <memory>
#include <any>

class BodyFactory {
public:
    BodyFactory();
    ~BodyFactory();

    IBody *createBody(enum CommandType_u type);
    size_t getBodySize(enum CommandType_u type);
    bool bodyExist(enum CommandType_u type);

protected:
private:
    std::unique_ptr<EmptyBody> _emptyBody;
    std::vector<std::unique_ptr<IBody>> _bodies;
};
