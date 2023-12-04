/*
** EPITECH PROJECT, 2023
** EntityComponentSystem
** File description:
** TestZipperIterator
*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "SparseArray.hpp"
#include "ZipperIterator.hpp"

TEST_CASE("ZipperIterator constructor", "[ZipperIterator]")
{
    SparseArray<int> intArray;
    intArray.insertAt(2, 4);
    SparseArray<double> doubleArray;
    doubleArray.insertAt(2, 4.4);
    std::tuple tuple = std::make_tuple(intArray.begin(), doubleArray.begin());
    ZipperIterator<SparseArray<int>, SparseArray<double>> it(tuple, intArray.size());
}

TEST_CASE("ZipperIterator operator++", "[ZipperIterator]")
{
    SparseArray<int> intArray;
    intArray.insertAt(2, 4);
    SparseArray<double> doubleArray;
    doubleArray.insertAt(2, 4.4);
    std::tuple tuple = std::make_tuple(intArray.begin(), doubleArray.begin());
    ZipperIterator<SparseArray<int>, SparseArray<double>> it(tuple, intArray.size());
    auto itd = (std::get<0>(*it));
    REQUIRE(itd == 4);
}

TEST_CASE("ZipperIterator operator==", "[ZipperIterator]")
{
    SparseArray<int> intArray;
    intArray.insertAt(2, 4);
    SparseArray<double> doubleArray;
    doubleArray.insertAt(2, 4.4);
    std::tuple tuple = std::make_tuple(intArray.begin(), doubleArray.begin());
    ZipperIterator<SparseArray<int>, SparseArray<double>> it(tuple, intArray.size());
    ZipperIterator<SparseArray<int>, SparseArray<double>> it2(tuple, intArray.size());
    REQUIRE(it == it2);
}

TEST_CASE("ZipperIterator operator!=", "[ZipperIterator]")
{
    SparseArray<int> intArray;
    intArray.insertAt(2, 4);
    SparseArray<double> doubleArray;
    doubleArray.insertAt(2, 4.4);
    std::tuple tuple = std::make_tuple(intArray.begin(), doubleArray.begin());
    ZipperIterator<SparseArray<int>, SparseArray<double>> it(tuple, intArray.size());
    ZipperIterator<SparseArray<int>, SparseArray<double>> it2(tuple, intArray.size());
    it2++;
    REQUIRE(it != it2);
}
