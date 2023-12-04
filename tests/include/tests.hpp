#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

class TestRand {
public:
    TestRand()
    {
        srand(time(NULL));
    }
    ~TestRand()
    {
    };

    int getRandInt(int min, int max) {
        return (rand() % (max - min + 1) + min);
    }

    uint16_t getRandPort() {
        return (getRandInt(1024, 65535));
    }
};

static TestRand rngRand;
