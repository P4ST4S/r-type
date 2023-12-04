/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** HandleGameStatusSystem
*/

#include "HandleGameStatusSystem.hpp"

HandleGameStatusSystem::HandleGameStatusSystem(ServerPackageHandler &serverPackageHandler, ClientHandler &clientHandler, serverState_t &state) :
_pkgHandler(serverPackageHandler), _cliHandler(clientHandler), _state(state)
{
}

HandleGameStatusSystem::~HandleGameStatusSystem()
{
}

void HandleGameStatusSystem::operator()(Registry &registry)
{
    // std::cout << "HANDLE GAME STATUS SYSTEM" << std::endl;
    if (this->_state == STATUS_GAME) {
        if (this->_cliHandler.generateCidList(TARGET_CLI_AUTH).empty())
            this->_state = STATUS_UNLOAD;
        this->handleMovementShootPackage(registry);

        std::deque<PartyEndEvent> events = registry.getEvent<PartyEndEvent>();
        if (!events.empty()) {
            Package pkg = this->_pkgHandler.createPackage(END_GAME);
            pkg.getBody<Body<EndGame_t>>().get()._win = events.back().getStatus();
            this->_pkgHandler.sendPackage(CPN_TCP, this->_cliHandler.generateCidList(TARGET_CLI_AUTH | TARGET_CLI_SPECTATOR), pkg);
            // this->_state = STATUS_UNLOAD;
        }
        registry.clearEvent<PartyEndEvent>();
        this->_pkgHandler.clearAllPackages();
    }
}

void HandleGameStatusSystem::handleMovementShootPackage(Registry &registry)
{
    std::deque<Package> pkgs;
    std::deque<cpn::connectionID_t> cids = this->_cliHandler.generateCidList(TARGET_CLI_AUTH);
    SparseArray<PlayerEventsComponent> &eventArray = registry.getComponents<PlayerEventsComponent>();

    while (!cids.empty()) {
        ClientInfo_t &cliInfo = this->_cliHandler.getClientInfo(cids.front());

        pkgs = PackageParser::extractPackages(
            this->_pkgHandler.accessPackages(cids.front()), {
            CLIENT_MOVE_EVENT
        });
        if (!pkgs.empty() && eventArray[cliInfo.entityId].has_value()) {
            Body<ClientMoveEvent_t> &body = pkgs.back().getBody<Body<ClientMoveEvent_t>>();
            eventArray[cliInfo.entityId].value().updateEvent(body.get());
            this->_pkgHandler.sendResponse(CPN_UDP, cids.front(), CLIENT_MOVE_EVENT, RESP_OK);
        }

        pkgs = PackageParser::extractPackages(
            this->_pkgHandler.accessPackages(cids.front()), {
            CLIENT_SHOOT_EVENT
        });
        if (!pkgs.empty() && eventArray[cliInfo.entityId].has_value()) {
            Body<ClientShootEvent_t> &body = pkgs.back().getBody<Body<ClientShootEvent_t>>();
            eventArray[cliInfo.entityId].value().updateEvent(body.get());
            this->_pkgHandler.sendResponse(CPN_UDP, cids.front(), CLIENT_SHOOT_EVENT, RESP_OK);
        }
        cids.pop_front();
    }
}
