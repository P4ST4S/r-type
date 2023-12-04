/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** CrossPlatformNetworkTest
*/

#include "tests.hpp"
#include "CrossPlatformNetwork.hpp"

TEST_CASE( "Test 1", "[CrossPlatformNetwork Function]" ) {
    uint32_t ip = cpn::Utils::ipStringToInt("0.0.0.1");
    std::string ipStr = cpn::Utils::ipIntToString(ip);

    REQUIRE( ip == 1 );
    REQUIRE( ipStr.compare("0.0.0.1") == 0 );
}
