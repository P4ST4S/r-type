/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** ClientClassTest
*/

#include "tests.hpp"
#include "CrossPlatformNetwork.hpp"

TEST_CASE( "Client Class basic initialization test", "[Client Class]" ) {
    cpn::Client client("127.0.0.1", 12345);
}

TEST_CASE( "Client Class initialization test", "[Client Class]" ) {
    uint16_t port = 12345;
    std::string exampleAddr = "127.0.0.1";
    cpn::Client client;

    REQUIRE( !client.connectTo("127.0.0.1", port) ); // Checking if a hard written string works.
    REQUIRE( !client.connectTo(exampleAddr, port) );

    cpn::Client client2("127.0.0.1", port); // Checking if a hard written string works.

    cpn::Client client3(exampleAddr, port);
}
