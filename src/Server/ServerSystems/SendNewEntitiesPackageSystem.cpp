/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** SendNewEntitiesPackageSystem
*/

#include "SendNewEntitiesPackageSystem.hpp"

SendNewEntitiesPackageSystem::SendNewEntitiesPackageSystem(ServerPackageHandler &serverPackageHandler, ClientHandler &clientHandler) :
_pkgHandler(serverPackageHandler), _cliHandler(clientHandler)

{
}

SendNewEntitiesPackageSystem::~SendNewEntitiesPackageSystem()
{
}


void SendNewEntitiesPackageSystem::operator()(Registry &registry)
{
    // std::cout << "SEND NEW ENTITIES PACKAGE SYSTEM" << std::endl;
    Package pkg = this->_pkgHandler.createPackage(NEW_ENTITIES);
    DynamicBody<NewEntities_t> &body = pkg.getBody<DynamicBody<NewEntities_t>>();
    std::deque<NewEntityEvent> events = registry.getEvent<NewEntityEvent>();
    NewEntities_t tmp;

    registry.clearEvent<NewEntityEvent>();
    for (NewEntityEvent &event : events) {
        tmp._id = event.getEntityId();
        tmp._type = event.getEntityType();
        body.add(tmp);
    }
    if (pkg.getBody<IBody>().size() > 0)
        this->_pkgHandler.sendPackage(CPN_TCP, this->_cliHandler.generateCidList(TARGET_CLI_AUTH | TARGET_CLI_SPECTATOR), pkg);
}
