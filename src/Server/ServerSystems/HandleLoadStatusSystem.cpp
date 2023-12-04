/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** HandleLoadStatusSystem
*/

#include "HandleLoadStatusSystem.hpp"

HandleLoadStatusSystem::HandleLoadStatusSystem(ServerPackageHandler &serverPackageHandler, ClientHandler &clientHandler, serverState_t &state) :
_pkgHandler(serverPackageHandler), _cliHandler(clientHandler), _state(state)
{
}

HandleLoadStatusSystem::~HandleLoadStatusSystem()
{
}

void HandleLoadStatusSystem::operator()(Registry &registry)
{
    // std::cout << "HANDLE LOAD STATUS SYSTEM" << std::endl;
    if (this->_state == STATUS_LOAD) {
        if (!this->_waitForClient) {
            // this->_pkgHandler.setLock(true);

            std::deque<cpn::connectionID_t> cids = this->_cliHandler.generateCidList(TARGET_CLI_AUTH);
            std::size_t i = 1;
            while (!cids.empty()) {
                ClientInfo_t &cliInfo = this->_cliHandler.getClientInfo(cids.front());

                cliInfo.entityCreated = true;
                cliInfo.readyToGame = false;
                cliInfo.entityId = NewEntityEvent::createNewEntity(registry, i);

                registry.addComponent<PlayerEventsComponent>(registry.entityFromIndex(cliInfo.entityId), PlayerEventsComponent());
                registry.addComponent<IdentityComponent>(registry.entityFromIndex(cliInfo.entityId), IdentityComponent(i));
                registry.addComponent<WeaponComponent>(registry.entityFromIndex(cliInfo.entityId), WeaponComponent(1, true, i));
                registry.addComponent<VelocityComponent>(registry.entityFromIndex(cliInfo.entityId), VelocityComponent(Vector2D<double>(70, 70)));
                registry.addComponent<LifeHitboxComponent>(registry.entityFromIndex(cliInfo.entityId), LifeHitboxComponent(Vector2D<double>(50, 50), 3));
                registry.addComponent<PlayerComponent>(registry.entityFromIndex(cliInfo.entityId), PlayerComponent());
                cids.pop_front();
                i++;
            }

            // JP Load your entities here
            // Entity boss = NewEntityEvent::createNewEntity(registry, GHOST_BOSS_ID);
            // registry.getComponents<InterpolationComponent>()[boss]->setPosition(2500, 400);
            // registry.addComponent<IdentityComponent>(boss, IdentityComponent(GHOST_BOSS_ID));
            // registry.addComponent<WeaponComponent>(boss, WeaponComponent(1, false, MOB_ID));
            // registry.addComponent<VelocityComponent>(boss, VelocityComponent(Vector2D<double>(50, 50)));
            // registry.addComponent<LifeHitboxComponent>(boss, LifeHitboxComponent(Vector2D<double>(1000, 1000), 5));
            // registry.addComponent<MobComponent>(boss, MobComponent());

            this->_waitForClient = true;
        }
        if (this->allClientReady()) {
            this->_state = STATUS_GAME;
            this->_pkgHandler.sendBodylessPackage(CPN_TCP, this->_cliHandler.generateCidList(TARGET_CLI_AUTH | TARGET_CLI_SPECTATOR), GAME_START);
            this->_waitForClient = false;
        }
        this->_pkgHandler.clearAllPackages();
    }
}

bool HandleLoadStatusSystem::allClientReady(void)
{
    bool allReady = true;
    std::deque<cpn::connectionID_t> cids = this->_cliHandler.generateCidList(TARGET_CLI_AUTH);

    while (!cids.empty()) {
        std::deque<Package> &pkgs = this->_pkgHandler.accessPackages(cids.front());
        std::deque<Package> pkgsReady = PackageParser::extractPackages(pkgs, {
            GAME_START
        });
        if (!pkgsReady.empty())
            this->_cliHandler.getClientInfo(cids.front()).readyToGame = true;
        if (!this->_cliHandler.getClientInfo(cids.front()).readyToGame)
            allReady = false;
        cids.pop_front();
    }
    return (allReady);
}
