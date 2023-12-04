/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** ServerClassTest
*/

#include "tests.hpp"
#include "CrossPlatformNetwork.hpp"

TEST_CASE( "Server Class initialization test", "[Server Class]" ) {
    size_t nbTest = 20;
    cpn::Server server;

    server.setLock(true);
    for (size_t i = 0; i < nbTest; ++i) {
        REQUIRE(!server.isReceptionThreadRunning());
        REQUIRE(server.start(rngRand.getRandPort()));
        REQUIRE(server.isReceptionThreadRunning());
        REQUIRE(server.stop());
        REQUIRE(!server.isReceptionThreadRunning());
    }
}

TEST_CASE( "Server Class configuration test", "[Server Class]" ) {
    uint16_t port = rngRand.getRandPort();
    cpn::Server server;

    REQUIRE( !server.getLock() );
    REQUIRE( server.getMaxConnection() == -1 );

    server.start(port);

    REQUIRE( server.getSocket(CPN_TCP) != -1 );
    REQUIRE( server.getSocket(CPN_UDP) != -1 );
    REQUIRE( server.getPort() == port );

    server.setLock(true);
    server.setMaxConnection(69);

    REQUIRE( server.getLock() );
    REQUIRE( server.getMaxConnection() == 69 );
}
