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
#include "PlayerComponent.hpp"
#include "RTypeServerStruct.hpp"
#include "Protocol.hpp"
#include "ClientHandler.hpp"
#include "WaveSystem.hpp"

class HandleBossAISystem {
    public:
        HandleBossAISystem(ServerPackageHandler &serverPackageHandler, ClientHandler &clientHandler);
        void operator()(Registry &registry,
            SparseArray<MobComponent> &mobsArray,
            SparseArray<BossComponent> &bossArray,
            SparseArray<VelocityComponent> &velocityArray,
            SparseArray<PlayerComponent> &playerArray,
            SparseArray<WeaponComponent> &weaponArray,
            SparseArray<InterpolationComponent> &interpolationArray);
    private:
        ServerPackageHandler &_pkgHandler;
        ClientHandler &_cliHandler;
};
