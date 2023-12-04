/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** WaveSystem
*/

#include "HandleMobAISystem.hpp"

HandleMobAISystem::HandleMobAISystem(ServerPackageHandler &serverPackageHandler, ClientHandler &clientHandler) :
_pkgHandler(serverPackageHandler), _cliHandler(clientHandler)
{
}

void HandleMobAISystem::operator()(Registry &registry,
SparseArray<MobComponent> &mobsArray,
SparseArray<VelocityComponent> &velocityArray,
SparseArray<PlayerComponent> &playerArray,
SparseArray<WeaponComponent> &weaponArray,
SparseArray<InterpolationComponent> &interpolationArray)
{
    bool shooted = false;
    Zipper<SparseArray<VelocityComponent>, SparseArray<MobComponent>, SparseArray<WeaponComponent>, SparseArray<InterpolationComponent>> mobs(velocityArray, mobsArray, weaponArray, interpolationArray);
    Zipper<SparseArray<PlayerComponent>, SparseArray<InterpolationComponent>> players(playerArray, interpolationArray);
    Package pkg = this->_pkgHandler.createPackage(ENTITY_ACTION);


    for (auto it = mobs.begin(); it != mobs.end(); it++) {
        size_t bestDist = UINT64_MAX;
        double dist;
        MobComponent &mob = std::get<MobComponent&>(*it);
        WeaponComponent &weapon = std::get<WeaponComponent&>(*it);
        InterpolationComponent &pos = std::get<InterpolationComponent&>(*it);
        VelocityComponent &velo = std::get<VelocityComponent&>(*it);
        weapon.updateCooldown();
        switch (mob._phase) {
        case 0:
            dist = 0;
            for (auto pit = players.begin(); pit != players.end(); pit++) {
                InterpolationComponent &ppos = std::get<InterpolationComponent&>(*pit);
                if (abs(ppos.getPosition()._y - pos.getPosition()._y) < bestDist) {
                    mob._targetPlayer = pit.getIndex();
                    dist = ppos.getPosition()._y - pos.getPosition()._y;
                    bestDist = abs(dist);
                }
            }
            if (bestDist < 50) {
                mob._phase = 1;
                mob._direction = (rand() % 2) ? 1 : -1;
                mob._moveFrame = 20 + rand() % 50;
                break;
            }
            if (dist != 0) {
                pos.move(-5, velo.getVelocity()._y * ((dist > 0) ? 1 : -1));
            }
            break;
        case 1:
            if (mob._nbrShot < 4) {
                shooted = weapon.shoot(registry, pos.getFinalPosition(), 6);
                if (shooted) {
                    ++(mob._nbrShot);
                    Body<EntityAction_t> &body = pkg.getBody<Body<EntityAction_t>>();
                    body.get()._id = it.getIndex();
                    body.get()._action = 0;
                    this->_pkgHandler.sendPackage(CPN_TCP, this->_cliHandler.generateCidList(TARGET_CLI_AUTH | TARGET_CLI_SPECTATOR), pkg);
                }
            } else {
                mob._nbrShot = 0;
                mob._phase = 2;
            }
            break;
        case 2:
            pos.move(5, velo.getVelocity()._y * mob._direction);
            if (mob._moveFrame <= 0) {
                mob._phase = 0;
            } else {
                --(mob._moveFrame);
            }
            break;
        }
        pos.replacePosition();
    }
}
