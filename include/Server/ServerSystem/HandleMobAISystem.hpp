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
#include "WeaponComponent.hpp"
#include "PlayerComponent.hpp"
#include "RTypeServerStruct.hpp"
#include "Protocol.hpp"
#include "ClientHandler.hpp"

class HandleMobAISystem {
    public:
        HandleMobAISystem(ServerPackageHandler &serverPackageHandler, ClientHandler &clientHandler);
        void operator()(Registry &registry, 
            SparseArray<MobComponent> &mobsArray, 
            SparseArray<VelocityComponent> &velocityArray, 
            SparseArray<PlayerComponent> &playerArray,
            SparseArray<WeaponComponent> &weaponArray,
            SparseArray<InterpolationComponent> &interpolationArray);
    private:
        ServerPackageHandler &_pkgHandler;
        ClientHandler &_cliHandler;
};
