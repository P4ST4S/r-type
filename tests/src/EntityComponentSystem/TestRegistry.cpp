/*
** EPITECH PROJECT, 2023
** EntityComponentSystem
** File description:
** TestRegistry
*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "Registry.hpp"

TEST_CASE("Registry spawn entity", "[Registry]")
{
    Registry registry;
    size_t entity1 = registry.spawnEntity();
    size_t entity2 = registry.spawnEntity();
    REQUIRE(entity1 == 0);
    REQUIRE(entity2 == 1);
}

TEST_CASE("Registry kill entity", "[Registry]")
{
    Registry registry;
    Entity entity1 = registry.spawnEntity();
    size_t entity2 = registry.spawnEntity();
    REQUIRE(entity1 == 0);
    registry.killEntity(entity1);
    size_t entity3 = registry.spawnEntity();
    REQUIRE(entity2 == 1);
    REQUIRE(entity3 == 0);
}

void func(Registry &registry, Entity const &entity)
{
    registry.killEntity(entity);
}

TEST_CASE("Registry add component", "[Registry]")
{
    Registry registry;
    Entity entity1 = registry.spawnEntity();
    registry.registerComponent<int>(func);
    registry.addComponent(entity1, 4);
    REQUIRE(registry.getComponents<int>()[entity1] == 4);
}

TEST_CASE("Registry emplace component", "[Registry]")
{
    Registry registry;
    Entity entity1 = registry.spawnEntity();
    registry.registerComponent<int>(func);
    registry.emplaceComponent<int>(entity1, 4);
    REQUIRE(registry.getComponents<int>()[entity1] == 4);
}

TEST_CASE("Registry get component", "[Registry]")
{
    Registry registry;
    Entity entity1 = registry.spawnEntity();
    registry.registerComponent<int>(func);
    registry.emplaceComponent<int>(entity1, 4);
    REQUIRE(registry.getComponents<int>()[entity1] == 4);
}

TEST_CASE("Registry remove component", "[Registry]")
{
    Registry registry;
    Entity entity1 = registry.spawnEntity();
    registry.registerComponent<int>(func);
    registry.emplaceComponent<int>(entity1, 4);
    registry.removeComponent<int>(entity1);
    REQUIRE(registry.getComponents<int>()[entity1].has_value() == false);
}

void increment_int_system(Registry &registry, SparseArray<int> &array)
{
    for (auto &elem : array) {
        if (elem.has_value())
            elem.value() += 1;
    }
}

void to_zero_system(Registry &registry, SparseArray<int> &array)
{
    for (auto &elem : array) {
        if (elem.has_value())
            elem.value() = 0;
    }
}

TEST_CASE("Registry add system", "[Registry]")
{
    Registry registry;
    Entity entity1 = registry.spawnEntity();
    registry.registerComponent<int>(func);
    registry.addComponent(entity1, 4);
    registry.addSystem<int>(increment_int_system);
    registry.runSystems();
    REQUIRE(registry.getComponents<int>()[entity1] == 5);
    registry.addSystem<int>(to_zero_system);
    registry.runSystems();
    REQUIRE(registry.getComponents<int>()[entity1] == 0);
}

