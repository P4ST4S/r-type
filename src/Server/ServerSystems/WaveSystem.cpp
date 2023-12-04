/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** WaveSystem
*/

#include "WaveSystem.hpp"

WaveSystem::WaveSystem(serverState_t &state) : _difficulty(0), _state(state)
{
}

void WaveSystem::operator()(Registry &registry, SparseArray<MobComponent> &mobsArray, SparseArray<BossComponent> &bossArray)
{
    if (this->_state != this->_prevState)
        this->_difficulty = 0;
    if (this->_state == STATUS_GAME && _difficulty <= 4) {
        Zipper<SparseArray<PlayerComponent>> players(registry.getComponents<PlayerComponent>());
        std::size_t nbPlayers = 0;

        for (auto it = players.begin(); it != players.end(); it++)
            nbPlayers++;
        if (nbPlayers == 0){
            PartyEndEvent::createPartyEnd(registry, false);
            return;
        }

        Zipper<SparseArray<MobComponent>> mobs(mobsArray);
        Zipper<SparseArray<BossComponent>> boss(bossArray);
        std::size_t nbMobs = 0;

        for (auto it = mobs.begin(); it != mobs.end(); it++)
            nbMobs++;
        for (auto it = boss.begin(); it != boss.end(); it++)
            nbMobs++;

        if (nbMobs <= 0){
            _difficulty++;
            if (_difficulty > 4) {
                PartyEndEvent::createPartyEnd(registry, true);
            } else if (_difficulty == 4) {
                Entity boss = NewEntityEvent::createNewEntity(registry, GHOST_BOSS_ID);

                registry.getComponents<InterpolationComponent>()[boss]->setPosition(2500, 400);
                registry.addComponent<IdentityComponent>(boss, IdentityComponent(GHOST_BOSS_ID));
                registry.addComponent<WeaponComponent>(boss, WeaponComponent(1, false, GHOST_BOSS_ID));
                registry.addComponent<VelocityComponent>(boss, VelocityComponent(Vector2D<double>(30, 30)));
                registry.addComponent<LifeHitboxComponent>(boss, LifeHitboxComponent(Vector2D<double>(1000, 1000), 250));
                registry.addComponent<BossComponent>(boss, BossComponent());
            } else {
                spawnWave(registry);
            }
        }
    }
    this->_prevState = this->_state;
}

void WaveSystem::spawnWave(Registry &registry)
{
    for (size_t i = 0; i < 7 + _difficulty; i++){
        WaveSystem::spawnEnemy(registry, this->_difficulty);
    }

}

void WaveSystem::spawnEnemy(Registry &registry, size_t difficulty)
{
    Entity entity = NewEntityEvent::createNewEntity(registry, MOB_ID);

    registry.getComponents<InterpolationComponent>()[entity]->setPosition((INTERNAL_WIDTH / 2) + (rand() % (INTERNAL_WIDTH / 2)), rand() % INTERNAL_HEIGHT);
    registry.addComponent<IdentityComponent>(entity, IdentityComponent(MOB_ID));
    registry.addComponent<WeaponComponent>(entity, WeaponComponent(1, false, MOB_ID));
    registry.addComponent<VelocityComponent>(entity, VelocityComponent(Vector2D<double>(50, 50)));
    registry.addComponent<LifeHitboxComponent>(entity, LifeHitboxComponent(Vector2D<double>(400, 400), 5 + difficulty));
    registry.addComponent<MobComponent>(entity, MobComponent());
}
