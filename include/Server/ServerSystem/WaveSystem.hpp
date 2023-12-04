/*
** EPITECH PROJECT, 2023
** GameEngine
** File description:
** WaveSystem
*/

#pragma once

#include "ISystem.hpp"
#include "GameEngine.hpp"
#include "MobComponent.hpp"
#include "BossComponent.hpp"
#include "WeaponComponent.hpp"
#include "PartyEndEvent.hpp"
#include "RTypeServerStruct.hpp"
#include "PlayerComponent.hpp"

class WaveSystem {
    public:
        WaveSystem(serverState_t &state);
        void operator()(Registry &registry, SparseArray<MobComponent> &mobsArray, SparseArray<BossComponent> &bossArray);
        static void spawnEnemy(Registry &registry, size_t difficulty);
    private:
        void spawnWave(Registry &registry);

        std::size_t _difficulty;
        serverState_t &_state;
        serverState_t _prevState;
};
