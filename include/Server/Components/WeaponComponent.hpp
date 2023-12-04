/*
** EPITECH PROJECT, 2023
** GameEngine
** File description:
** BasicWeapon
*/

#pragma once

#include "EntityEnums.hpp"

#include "IComponent.hpp"
#include "Vector2D.hpp"
#include "VelocityComponent.hpp"
#include "InterpolationComponent.hpp"
#include "DamageHitboxComponent.hpp"
#include "PositionComponent.hpp"
#include "IdentityComponent.hpp"
#include "BulletComponent.hpp"
#include "NewEntityEvent.hpp"
#include "EntityEnums.hpp"

class WeaponComponent : public IComponent{
    public:
        /**
         * @brief Construct a new weapon for an entity
         *
         * @param damage Damages of the weapon
         * @param direction Direction where the projectile will go, true for right, false for left
         */
        WeaponComponent(std::size_t damage, bool direction, int owner);

        /**
         * @brief The weapon will create a new entity projectile the will be construct according to the parameters of the weapon
         *
         * @param registry Registry of the game, used to create the projectile
         * @param position Position of the current weapon, used to spawn the projectile at the right place
         */
        bool shoot(Registry &registry, Vector2D<double> position, int cooldown);

        void updateCooldown();
    private:
        std::size_t _damages; // Damages of the weapon
        bool _direction; // Direction where the projectile will go, true for right, false for left
        int _owner;
        size_t _coolDown = 0;

};

/**
 * @brief Destroy the component BasicWeaponComponent of an entity
 *
 * @param registry Registry of the game, used to destroy the component
 * @param entity Entity which will lose the component
 */
void destroyWeapon(Registry &registry, Entity const &entity);
