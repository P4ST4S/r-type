/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** HandleBossAISystem
*/

#include "HandleBossAISystem.hpp"

HandleBossAISystem::HandleBossAISystem(ServerPackageHandler &serverPackageHandler, ClientHandler &clientHandler) :
_pkgHandler(serverPackageHandler), _cliHandler(clientHandler)
{
}

void HandleBossAISystem::operator()(Registry &registry,
SparseArray<MobComponent> &mobsArray,
SparseArray<BossComponent> &bossArray,
SparseArray<VelocityComponent> &velocityArray,
SparseArray<PlayerComponent> &playerArray,
SparseArray<WeaponComponent> &weaponArray,
SparseArray<InterpolationComponent> &interpolationArray)
{
    bool shooted = false;
    Zipper<SparseArray<VelocityComponent>, SparseArray<BossComponent>, SparseArray<WeaponComponent>, SparseArray<InterpolationComponent>> bosses(velocityArray, bossArray, weaponArray, interpolationArray);
    Zipper<SparseArray<PlayerComponent>, SparseArray<InterpolationComponent>> players(playerArray, interpolationArray);
    Package pkg = this->_pkgHandler.createPackage(ENTITY_ACTION);


    for (auto it = bosses.begin(); it != bosses.end(); it++) {
        size_t bestDist = UINT64_MAX;
        double dist;
        BossComponent &boss = std::get<BossComponent&>(*it);
        WeaponComponent &weapon = std::get<WeaponComponent&>(*it);
        InterpolationComponent &pos = std::get<InterpolationComponent&>(*it);
        VelocityComponent &velo = std::get<VelocityComponent&>(*it);
        weapon.updateCooldown();
        switch (boss._phase) {
        case 0:
            dist = 0;
            for (auto pit = players.begin(); pit != players.end(); pit++) {
                InterpolationComponent &ppos = std::get<InterpolationComponent&>(*pit);
                if (abs(ppos.getPosition()._y - pos.getPosition()._y) < bestDist) {
                    boss._targetPlayer = pit.getIndex();
                    dist = ppos.getPosition()._y - pos.getPosition()._y;
                    bestDist = abs(dist);
                }
            }
            if (bestDist < 150) {
                boss._phase = 1;
                boss._direction = (rand() % 2) ? 1 : -1;
                boss._moveFrame = 20 + rand() % 30;
                boss._actionType = rand() % 2;
                break;
            }
            if (dist != 0) {
                pos.move(-5, velo.getVelocity()._y * ((dist > 0) ? 1 : -1));
            }

            break;
        case 1:
            if (boss._actionType) {
                Vector2D<double> finalPos = pos.getFinalPosition();
                finalPos._y += (boss._nbrShot - 2) * 240;
                boss._nbrShot += weapon.shoot(registry, finalPos, 1);
                if (boss._nbrShot > 5) {
                    boss._nbrShot = 0;
                    boss._phase = 2;
                }
            } else {
                Zipper<SparseArray<MobComponent>> mobs(mobsArray);
                std::size_t nbMobs = 0;
                for (auto it = mobs.begin(); it != mobs.end(); it++)
                    nbMobs++;
                for (size_t i = nbMobs; i < 5; i++) {
                    WaveSystem::spawnEnemy(registry, 4);
                }
                boss._phase = 2;
            }
            break;
        case 2:
            pos.move(5, velo.getVelocity()._y * boss._direction);
            if (boss._moveFrame <= 0) {
                boss._phase = 0;
            } else {
                --(boss._moveFrame);
            }
            break;
        }
        pos.replacePosition();
    }
}
