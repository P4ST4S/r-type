/*
** EPITECH PROJECT, 2023
** EntityComponentSystem
** File description:
** TestZipper
*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "Zipper.hpp"
#include "SparseArray.hpp"

TEST_CASE("Zipper", "[Zipper]")
{
    SparseArray<int> iarray;
    SparseArray<std::string> sarray;
    iarray.insertAt(2, 3);
    iarray.insertAt(5, 2);
    sarray.insertAt(2, "three");
    sarray.insertAt(5, "two");
    sarray.insertAt(10, "alone");
    Zipper<SparseArray<int>, SparseArray<std::string>> zipper(iarray, sarray);

    size_t allset = 0;
    auto begin = zipper.begin();
    while (begin != zipper.end()){
        begin++;
        allset++;
    }
    REQUIRE(begin == zipper.end());
    REQUIRE(allset == 2);
}

