/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** HandleEntity
*/

#pragma once

#include <unordered_map>

#include "Registry.hpp"
#include "Entity.hpp"
#include "Protocol.hpp"
#include "InterpolationComponent.hpp"
#include "PositionComponent.hpp"
#include "TextureBank.hpp"
#include "TextureComponent.hpp"
#include "EntityEnums.hpp"

class HandleEntity {
    public:
        HandleEntity(Registry &registry, std::shared_ptr<RGraphic::IGraphic> &graphic);
        ~HandleEntity();

        std::size_t createEntity(NewEntities_t idFromPkgs);
        std::size_t replaceEntity(NewEntities_t idFromPkgs);
        void destroyEntity(std::size_t id);
        double moveEntity(MoveEntitiesDyn_s &moveEntities, std::chrono::duration<double> &timeStamp);

        bool entityExist(std::size_t id) const;
        std::size_t getEntity(std::size_t id) const;

        void packetsInterpreter(std::deque<Package> &packets);
        void updateNewEntities(Package &package);
        void updateDestroyEntities(Package &package);
        void updateMoveEntities(Package &package);
        void UpdatedyingEntities();

        void resetFirstMovePkg();
    private:
        bool _firstMovePkg = true;
        std::chrono::duration<double> _startTime;
        std::chrono::duration<double> _lastTime = std::chrono::duration<double>::zero();
        std::chrono::_V2::system_clock::time_point _timeNow;
        std::unordered_map<int, RGraphic::RTexture> _bank;
        uint8_t _order1 = 0;
        uint8_t _order2 = 0;
        Registry &_registry;
        std::vector<size_t> _dyingEntities;
        std::unordered_map<std::size_t, std::size_t> _entities;
        std::shared_ptr<RGraphic::IGraphic> _graphic;
};
