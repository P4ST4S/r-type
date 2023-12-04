/*
** EPITECH PROJECT, 2023
** EntityComponentSystem
** File description:
** TestSparseArray
*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <SparseArray.hpp>

TEST_CASE("SparseArray copy constructor", "[SparseArray]")
{
    SparseArray<int *> array;
    int *ptr1 = new int(4);
    
    array.insertAt(2, ptr1);
    SparseArray<int *> copy(array);
    REQUIRE(copy.size() == 3);
    REQUIRE(copy[2] == ptr1);
    REQUIRE(array.size() == 3);
    REQUIRE(array[2] == ptr1);
}

TEST_CASE("SparseArray move constructor", "[SparseArray]")
{
    SparseArray<int *> array;
    int *ptr1 = new int(4);
    
    array.insertAt(2, ptr1);
    SparseArray<int *> move{std::move(array)};
    REQUIRE(move.size() == 3);
    REQUIRE(move[2] == ptr1);
    REQUIRE(array.size() != 3);
}

TEST_CASE("SparseArray copy assignment", "[SparseArray]")
{
    SparseArray<int *> array;
    int *ptr1 = new int(4);
    
    array.insertAt(2, ptr1);
    SparseArray<int *> copy;
    copy = array;
    REQUIRE(copy.size() == 3);
    REQUIRE(copy[2] == ptr1);
    REQUIRE(array.size() == 3);
    REQUIRE(array[2] == ptr1);
}

TEST_CASE("SparseArray move assignment", "[SparseArray]")
{
    SparseArray<int *> array;
    int *ptr1 = new int(4);
    
    array.insertAt(2, ptr1);
    SparseArray<int *> move;
    move = std::move(array);
    REQUIRE(move.size() == 3);
    REQUIRE(move[2] == ptr1);
    REQUIRE(array.size() != 3);
}

TEST_CASE("SparseArray insertAt", "[SparseArray]")
{
    SparseArray<int> array;
    int value1 = 4;
    int value2 = 5;
    int value3 = 6;
    
    array.insertAt(2, value1);
    array.insertAt(4, value2);
    array.insertAt(6, value3);
    REQUIRE(array.size() == 7);
    REQUIRE(array[2] == value1);
    REQUIRE(array[4] == value2);
    REQUIRE(array[6] == value3);

    value1 = 7;
    value2 = 8;
    value3 = 9;
    
    array.insertAt(2, value1);
    array.insertAt(4, value2);
    array.insertAt(6, value3);
    REQUIRE(array.size() == 7);
    REQUIRE(array[2] == value1);
    REQUIRE(array[4] == value2);
    REQUIRE(array[6] == value3);
}

TEST_CASE("SparseArray emplaceAt", "[SparseArray]")
{
    SparseArray<int> array;
    int value1 = 4;
    int value2 = 5;
    int value3 = 6;
    
    array.emplaceAt(2, value1);
    array.emplaceAt(4, value2);
    array.emplaceAt(6, value3);
    REQUIRE(array.size() == 7);
    REQUIRE(array[2] == value1);
    REQUIRE(array[4] == value2);
    REQUIRE(array[6] == value3);

    value1 = 7;
    value2 = 8;
    value3 = 9;
    
    array.emplaceAt(2, value1);
    array.emplaceAt(4, value2);
    array.emplaceAt(6, value3);
    REQUIRE(array.size() == 7);
    REQUIRE(array[2] == value1);
    REQUIRE(array[4] == value2);
    REQUIRE(array[6] == value3);
}

TEST_CASE("SparseArray iterator", "[SparseArray]")
{
    SparseArray<int> array;
    int value1 = 4;
    int value2 = 5;
    int value3 = 6;
    
    array.insertAt(2, value1);
    array.insertAt(4, value2);
    array.insertAt(6, value3);
    auto it = array.begin();
    it++;
    it++;
    REQUIRE((*(it++)) == value1);
    REQUIRE((*(it++)).has_value() == false);
    REQUIRE((*(it++)) == value2);
    REQUIRE((*(it++)).has_value() == false);
    REQUIRE((*(it++)) == value3);
    REQUIRE(it == array.end());
}

TEST_CASE("SparseArray const iterator", "[SparseArray]")
{
    SparseArray<int> array;
    int value1 = 4;
    int value2 = 5;
    int value3 = 6;
    
    array.insertAt(2, value1);
    array.insertAt(4, value2);
    array.insertAt(6, value3);
    auto it = array.cbegin();
    it++;
    it++;
    REQUIRE((*(it++)) == value1);
    REQUIRE((*(it++)).has_value() == false);
    REQUIRE((*(it++)) == value2);
    REQUIRE((*(it++)).has_value() == false);
    REQUIRE((*(it++)) == value3);
    REQUIRE(it == array.cend());
}

TEST_CASE("SparseArray erase", "[SparseArray]")
{
    SparseArray<int> array;
    int value1 = 4;
    int value2 = 5;
    int value3 = 6;
    
    array.insertAt(2, value1);
    array.insertAt(4, value2);
    array.insertAt(6, value3);
    array.erase(2);
    REQUIRE(array.size() == 7);
    REQUIRE(array[2].has_value() == false);
    REQUIRE(array[4] == value2);
    REQUIRE(array[6] == value3);
}

TEST_CASE("SparseArray getIndex", "[SparseArray]")
{
    SparseArray<int> array;
    int value1 = 4;
    int value2 = 5;
    int value3 = 6;
    
    array.insertAt(2, value1);
    array.insertAt(4, value2);
    array.insertAt(6, value3);
    REQUIRE(array.getIndex(value1) == 2);
    REQUIRE(array.getIndex(value2) == 4);
    REQUIRE(array.getIndex(value3) == 6);
    REQUIRE(array.getIndex(1) == array.size());
}