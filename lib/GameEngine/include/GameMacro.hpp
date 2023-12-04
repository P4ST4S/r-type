/*
** EPITECH PROJECT, 2023
** GameEngine
** File description:
** Macro
*/

#pragma once

#include "Vector2D.hpp"

//Game macros
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

//Diffculty macros
#define EASY 1
#define MEDIUM 2
#define HARD 3

//Player macros
#define PLAYER_SPAWN_POSITION ({Vector2D<double>(rand() % 960, rand() % 1080);})
#define PLAYER_HITBOX ({Vector2D<double>(64, 64);})
#define PLAYER_VELOCITY ({Vector2D<double>(5, 5);})
#define PLAYER_HEALTH 500
#define PLAYER_DAMAGE 10
#define PLAYER_DIRECTION true

//Mob macros
#define MOB_SPAWN_POSITION ({Vector2D<double>(rand() % 960 + 960, rand() % 1080);})
#define MOB_HITBOX ({Vector2D<double>(64, 64);})
#define MOB_VELOCITY ({Vector2D<double>(5, 5);})
#define MOB_HEALTH 100
#define MOB_DAMAGE 5
#define MOB_DIRECTION false

enum EntityType {
    PLAYER,
    MOB
};
