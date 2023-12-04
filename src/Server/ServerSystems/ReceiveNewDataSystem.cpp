/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** ReceiveNewDataSystem
*/

#include "ReceiveNewDataSystem.hpp"

ReceiveNewDataSystem::ReceiveNewDataSystem(ServerPackageHandler &serverPackageHandler) :
_pkgHandler(serverPackageHandler)

{
}

ReceiveNewDataSystem::~ReceiveNewDataSystem()
{
}


void ReceiveNewDataSystem::operator()(Registry &registry)
{
    // std::cout << "--------- RECEIVE NEW DATA SYSTEM ---------" << std::endl;
    this->_pkgHandler.receiveNewData();
    // std::cout << "--------- RECEIVE NEW DATA SYSTEM 2 ---------" << std::endl;
}
