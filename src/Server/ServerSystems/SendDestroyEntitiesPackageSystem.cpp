/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** SendDestroyEntitiesPackageSystem
*/

#include "SendDestroyEntitiesPackageSystem.hpp"

SendDestroyEntitiesPackageSystem::SendDestroyEntitiesPackageSystem(ServerPackageHandler &serverPackageHandler, ClientHandler &clientHandler) :
_pkgHandler(serverPackageHandler), _cliHandler(clientHandler)

{
}

SendDestroyEntitiesPackageSystem::~SendDestroyEntitiesPackageSystem()
{
}


void SendDestroyEntitiesPackageSystem::operator()(Registry &registry)
{
    // std::cout << "SEND DESTROY ENTITIES PACKAGE SYSTEM" << std::endl;
    Package pkg = this->_pkgHandler.createPackage(DESTROY_ENTITIES);
    DynamicBody<DestroyEntities_t> &body = pkg.getBody<DynamicBody<DestroyEntities_t>>();
    std::deque<KillEntityEvent> events = registry.getEvent<KillEntityEvent>();
    DestroyEntities_t tmp;

    registry.clearEvent<KillEntityEvent>();
    for (KillEntityEvent &event : events) {
        tmp._id = event.getEntityId();
        body.add(tmp);
    }
    if (pkg.getBody<IBody>().size() > 0)
        this->_pkgHandler.sendPackage(CPN_TCP, this->_cliHandler.generateCidList(TARGET_CLI_AUTH | TARGET_CLI_SPECTATOR), pkg);
}
