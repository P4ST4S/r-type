/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** CrossPlatformNetworkTest
*/

#include "tests.hpp"
#include "CrossPlatformNetwork.hpp"

TEST_CASE( "Simple package exchange TCP/UDP", "[CrossPlatformNetwork]" ) {
    uint16_t port = rngRand.getRandPort();
    std::string ip = "127.0.0.1";
    cpn::Server server;
    cpn::Client client;

    std::string msg = "Hello World!";
    std::deque<uint8_t> msgData(msg.begin(), msg.end());
    int msgLen = msg.length() + 1;
    char getMsg[msgLen];
    std::deque<uint8_t> data;
    std::deque<cpn::connectionID_t> clientList;
    cpn::connectionID_t cid;
    int protocol = CPN_TCP;

    REQUIRE(server.start(port));
    REQUIRE(client.connectTo(ip, port));
    for (size_t i = 0; i < 2; ++i) {
        REQUIRE(client.send(protocol, msgData, msgLen) == msgLen);
        cpn::Utils::msleep(1);
        clientList = server.getConnectionList();
        REQUIRE(clientList.size() == 1);
        cid = clientList[0];
        data = server.receive(protocol, cid, msgLen);
        REQUIRE(data.size() == msgLen);
        memcpy(getMsg, DEQUE_DATA(data), msgLen);
        REQUIRE(msg.compare(getMsg) == 0);

        protocol = CPN_UDP;
    }
    server.stop();
}

TEST_CASE( "Simple package exchange using receiveFromAny TCP/UDP", "[CrossPlatformNetwork]" ) {
    uint16_t port = rngRand.getRandPort();
    std::string ip = "127.0.0.1";
    std::string msg = "Hello World!";
    std::deque<uint8_t> msgData(msg.begin(), msg.end());
    int msgLen = msg.length() + 1;
    char getMsg[msgLen];
    cpn::Client client;
    cpn::Server server;
    std::deque<uint8_t> data;
    std::deque<cpn::connectionID_t> clientList;
    cpn::connectionID_t cid;
    int protocol = CPN_TCP;

    REQUIRE( server.start(port) );
    REQUIRE( client.connectTo(ip, port) );
    for (size_t i = 0; i < 2; ++i) {
        REQUIRE(client.send(protocol, msgData, msgLen) == msgLen);
        cpn::Utils::msleep(1);
        data = server.receiveFromAny(protocol, cid, msgLen);
        REQUIRE(data.size() == msgLen);
        memcpy(getMsg, DEQUE_DATA(data), msgLen);
        REQUIRE(msg.compare(getMsg) == 0);

        protocol = CPN_UDP;
    }
    server.stop();
}

TEST_CASE( "Multiple package exchange with multiple clients TCP/UDP", "[CrossPlatformNetwork]" ) {
    uint16_t port = rngRand.getRandPort();
    std::string ip = "127.0.0.1";
    int msgLen = 2;
    bool fourCheck[4];
    cpn::Server server;

    cpn::Client client1;
    cpn::Client client2;
    cpn::Client client3;
    cpn::Client client4;

    std::deque<uint8_t> data;
    std::deque<cpn::connectionID_t> clientList;
    cpn::connectionID_t cid;
    int protocol = CPN_TCP;


    REQUIRE( server.start(port) );
    REQUIRE( client1.connectTo(ip, port) );
    REQUIRE( client2.connectTo(ip, port) );
    REQUIRE( client3.connectTo(ip, port) );
    REQUIRE( client4.connectTo(ip, port) );

    for (size_t i = 0; i < 2; ++i) {
        memset(fourCheck, 0, 4);

        REQUIRE(client1.send(protocol, {'1', 0}, msgLen) == msgLen);
        cpn::Utils::msleep(1);
        REQUIRE(client2.send(protocol, {'2', 0}, msgLen) == msgLen);
        cpn::Utils::msleep(1);
        REQUIRE(client3.send(protocol, {'3', 0}, msgLen) == msgLen);
        cpn::Utils::msleep(1);
        REQUIRE(client4.send(protocol, {'4', 0}, msgLen) == msgLen);
        cpn::Utils::msleep(1);

        clientList = server.getConnectionList();
        REQUIRE(clientList.size() == 4);

        for (int i = 0; i < 4; ++i) {
            data = server.receiveFromAny(protocol, cid, msgLen);
            REQUIRE(data.size() == msgLen);
            if (memcmp(DEQUE_DATA(data), "1", msgLen) == 0) {
                REQUIRE(!fourCheck[0]);
                fourCheck[0] = true;
            } else if (memcmp(DEQUE_DATA(data), "2", msgLen) == 0) {
                REQUIRE(!fourCheck[1]);
                fourCheck[1] = true;
            } else if (memcmp(DEQUE_DATA(data), "3", msgLen) == 0) {
                REQUIRE(!fourCheck[2]);
                fourCheck[2] = true;
            } else if (memcmp(DEQUE_DATA(data), "4", msgLen) == 0) {
                REQUIRE(!fourCheck[3]);
                fourCheck[3] = true;
            }
            REQUIRE(fourCheck[0] + fourCheck[1] + fourCheck[2] + fourCheck[3] == i + 1);
            if (protocol == CPN_TCP)
                REQUIRE(server.getConnection(cid).tcpBuf.size() == 0);
            if (protocol == CPN_UDP)
                REQUIRE(server.getConnection(cid).udpBuf.size() == 0);
        }
        data = server.receiveFromAny(protocol, cid, msgLen);
        REQUIRE(data.size() == 0);
        REQUIRE(cid._valid == 0);
        protocol = CPN_UDP;
    }
    server.stop();
}

TEST_CASE( "Connection limit by max client TCP/UDP", "[CrossPlatformNetwork]" ) {
    uint16_t port = rngRand.getRandPort();
    std::string ip = "127.0.0.1";
    int msgLen = 2;
    bool fourCheck[2];

    cpn::Server server;
    cpn::Client client1;
    cpn::Client client2;
    cpn::Client client3;
    cpn::Client client4;

    std::deque<uint8_t> data;
    std::deque<cpn::connectionID_t> clientList;
    cpn::connectionID_t cid;
    int protocol = CPN_TCP;

    server.start(port);
    server.setMaxConnection(2);

    REQUIRE( client1.connectTo(ip, port) );
    REQUIRE( client2.connectTo(ip, port) );
    REQUIRE( client3.connectTo(ip, port) );
    REQUIRE( client4.connectTo(ip, port) );


    for (size_t i = 0; i < 2; ++i) {
        memset(fourCheck, 0, 2);

        REQUIRE(client1.send(protocol, {'1', 0}, msgLen) == msgLen);
        cpn::Utils::msleep(1);
        REQUIRE(client2.send(protocol, {'2', 0}, msgLen) == msgLen);
        cpn::Utils::msleep(1);
        REQUIRE(client3.send(protocol, {'3', 0}, msgLen) == msgLen);
        cpn::Utils::msleep(1);
        REQUIRE(client4.send(protocol, {'4', 0}, msgLen) == msgLen);
        cpn::Utils::msleep(1);

        clientList = server.getConnectionList();
        REQUIRE(clientList.size() == 2);

        for (int i = 0; i < 2; ++i) {
            data = server.receiveFromAny(protocol, cid, msgLen);
            REQUIRE(data.size() == msgLen);
            if (memcmp(DEQUE_DATA(data), "1", msgLen) == 0) {
                REQUIRE(!fourCheck[0]);
                fourCheck[0] = true;
            } else if (memcmp(DEQUE_DATA(data), "2", msgLen) == 0) {
                REQUIRE(!fourCheck[1]);
                fourCheck[1] = true;
            }
            REQUIRE(fourCheck[0] + fourCheck[1] == i + 1);
            if (protocol == CPN_TCP)
                REQUIRE(server.getConnection(cid).tcpBuf.size() == 0);
            if (protocol == CPN_UDP)
                REQUIRE(server.getConnection(cid).udpBuf.size() == 0);
        }
        data = server.receiveFromAny(protocol, cid, msgLen);
        REQUIRE(data.size() == 0);
        protocol = CPN_UDP;
    }
    server.stop();
}

TEST_CASE( "Connection limit by max client TCP/UDP 2", "[CrossPlatformNetwork]" ) {
    uint16_t port = rngRand.getRandPort();
    std::string ip = "127.0.0.1";
    int msgLen = 2;
    bool fourCheck[2];

    cpn::Server server;
    cpn::Client client1;
    cpn::Client client2;
    cpn::Client client3;
    cpn::Client client4;

    std::deque<uint8_t> data;
    std::deque<cpn::connectionID_t> clientList;
    cpn::connectionID_t cid;
    int protocol = CPN_TCP;

    server.start(port);
    server.setMaxConnection(2);

    REQUIRE( client1.connectTo(ip, port) );
    REQUIRE( client2.connectTo(ip, port) );
    REQUIRE( client3.connectTo(ip, port) );
    // Client 4 is intentionally not connected


    for (size_t i = 0; i < 2; ++i) {
        memset(fourCheck, 0, 2);

        REQUIRE(client1.send(protocol, {'1', 0}, msgLen) == msgLen);
        cpn::Utils::msleep(1);
        REQUIRE(client2.send(protocol, {'2', 0}, msgLen) == msgLen);
        cpn::Utils::msleep(1);
        REQUIRE(client3.send(protocol, {'3', 0}, msgLen) == msgLen);
        cpn::Utils::msleep(1);
        // try {
        //     client4.send(protocol, {'4', 0}, msgLen) == msgLen;
        //     REQUIRE(false);
        // } catch (std::exception& e) {
        //     REQUIRE(true);
        // }

        clientList = server.getConnectionList();
        REQUIRE(clientList.size() == 2);

        for (int i = 0; i < 2; ++i) {
            data = server.receiveFromAny(protocol, cid, msgLen);
            REQUIRE(data.size() == msgLen);
            if (memcmp(DEQUE_DATA(data), "1", msgLen) == 0) {
                REQUIRE(!fourCheck[0]);
                fourCheck[0] = true;
            } else if (memcmp(DEQUE_DATA(data), "2", msgLen) == 0) {
                REQUIRE(!fourCheck[1]);
                fourCheck[1] = true;
            }
            REQUIRE(fourCheck[0] + fourCheck[1] == i + 1);
            if (protocol == CPN_TCP)
                REQUIRE(server.getConnection(cid).tcpBuf.size() == 0);
            if (protocol == CPN_UDP)
                REQUIRE(server.getConnection(cid).udpBuf.size() == 0);
        }
        data = server.receiveFromAny(protocol, cid, msgLen);
        REQUIRE(data.size() == 0);
        protocol = CPN_UDP;
    }
    server.stop();
}

TEST_CASE( "Connection limit by lock & max client UDP/TCP", "[CrossPlatformNetwork]" ) {
    uint16_t port = rngRand.getRandPort();
    std::string ip = "127.0.0.1";
    int msgLen = 2;
    bool fourCheck[2];

    cpn::Client client1;
    cpn::Client client2;
    cpn::Client client3;
    cpn::Client client4;
    cpn::Server server;

    std::deque<uint8_t> data;
    std::deque<cpn::connectionID_t> clientList;
    cpn::connectionID_t cid;
    int protocol = CPN_TCP;

    REQUIRE( server.start(port) );
    server.setMaxConnection(2);

    REQUIRE( client1.connectTo(ip, port) );
    REQUIRE( client2.connectTo(ip, port) );
    cpn::Utils::msleep(10);
    server.setLock(true);
    REQUIRE( client3.connectTo(ip, port) );
    REQUIRE( client4.connectTo(ip, port) );

    for (size_t i = 0; i < 2; ++i) {
        memset(fourCheck, 0, 2);

        REQUIRE(client1.send(protocol, {'1', 0}, msgLen) == msgLen);
        cpn::Utils::msleep(1);
        REQUIRE(client2.send(protocol, {'2', 0}, msgLen) == msgLen);
        cpn::Utils::msleep(1);
        REQUIRE(client3.send(protocol, {'3', 0}, msgLen) == msgLen);
        cpn::Utils::msleep(1);
        REQUIRE(client4.send(protocol, {'4', 0}, msgLen) == msgLen);
        cpn::Utils::msleep(1);

        REQUIRE(server.getConnectionCount() == 2);

        for (int i = 0; i < 2; ++i) {
            data = server.receiveFromAny(protocol, cid, msgLen);
            std::cout << "data.size() = " << data.size() << std::endl;
            REQUIRE(data.size() == msgLen);
            if (memcmp(DEQUE_DATA(data), "1", msgLen) == 0) {
                REQUIRE(!fourCheck[0]);
                fourCheck[0] = true;
            } else if (memcmp(DEQUE_DATA(data), "2", msgLen) == 0) {
                REQUIRE(!fourCheck[1]);
                fourCheck[1] = true;
            }
            REQUIRE(fourCheck[0] + fourCheck[1] == i + 1);
            if (protocol == CPN_TCP)
                REQUIRE(server.getConnection(cid).tcpBuf.size() == 0);
            if (protocol == CPN_UDP)
                REQUIRE(server.getConnection(cid).udpBuf.size() == 0);
        }
        data = server.receiveFromAny(protocol, cid, msgLen);
        REQUIRE(data.size() == 0);
        protocol = CPN_UDP;
    }
    server.stop();
}

TEST_CASE( "Simple echo TCP/UDP", "[CrossPlatformNetwork]" ) {
    uint16_t port = rngRand.getRandPort();
    std::string ip = "127.0.0.1";
    std::string msg = "Hello World!";
    std::deque<uint8_t> msgData(msg.begin(), msg.end());
    int msgLen = msg.length() + 1;
    char getMsg[msgLen];
    cpn::Client client;
    cpn::Server server;
    std::deque<uint8_t> data;
    std::deque<cpn::connectionID_t> clientList;
    cpn::connectionID_t cid;
    int protocol = CPN_TCP;

    REQUIRE( server.start(port) );
    REQUIRE( client.connectTo(ip, port) );

    for (size_t i = 0; i < 2; ++i) {
        REQUIRE(client.send(protocol, msgData, msgLen) == msgLen);
        cpn::Utils::msleep(1);
        clientList = server.getConnectionList();
        REQUIRE(server.getConnectionCount() == 1);
        REQUIRE(clientList.size() == 1);
        cid = clientList[0];
        data = server.receive(protocol, cid, msgLen);
        REQUIRE(data.size() == msgLen);
        memcpy(getMsg, DEQUE_DATA(data), msgLen);
        REQUIRE(msg.compare(getMsg) == 0);
        server.send(protocol, cid, data, msgLen);
        cpn::Utils::msleep(1);

        data = client.receive(protocol, msgLen);
        memcpy(getMsg, DEQUE_DATA(data), msgLen);
        REQUIRE(data.size() == msgLen);
        REQUIRE(msg.compare(getMsg) == 0);
        data = client.receive(protocol, msgLen);
        REQUIRE(data.size() == 0);
        protocol = CPN_UDP;
    }

    server.stop();
}

TEST_CASE( "Simple echo 2 TCP/UDP", "[CrossPlatformNetwork]" ) {
    uint16_t port = rngRand.getRandPort();
    std::string ip = "127.0.0.1";
    std::string msg = "Hello World!";
    std::deque<uint8_t> msgData(msg.begin(), msg.end());
    int msgLen = msg.length() + 1;
    char getMsg[msgLen];
    cpn::Client client;
    cpn::Server server;
    std::deque<uint8_t> data;
    std::deque<cpn::connectionID_t> clientList;
    cpn::connectionID_t cid;
    int protocol = CPN_TCP;

    REQUIRE( server.start(port) );
    REQUIRE( client.connectTo(ip, port) );

    for (size_t i = 0; i < 2; ++i) {
        REQUIRE(client.send(protocol, msgData, msgLen) == msgLen);
        cpn::Utils::msleep(1);

        clientList = server.getConnectionList();
        REQUIRE(clientList.size() == 1);
        cid = clientList[0];
        data = server.receive(protocol, cid, msgLen);
        REQUIRE(data.size() == msgLen);
        memcpy(getMsg, DEQUE_DATA(data), msgLen);
        REQUIRE(msg.compare(getMsg) == 0);
        server.send(protocol, cid, data, msgLen);
        cpn::Utils::msleep(1);

        data = client.receive(protocol, msgLen);
        memcpy(getMsg, DEQUE_DATA(data), msgLen);
        REQUIRE(data.size() == msgLen);
        REQUIRE(msg.compare(getMsg) == 0);


        REQUIRE(client.send(protocol, data) == msgLen);
        cpn::Utils::msleep(1);

        clientList = server.getConnectionList();
        REQUIRE(clientList.size() == 1);
        cid = clientList[0];
        data = server.receive(protocol, cid, msgLen);
        REQUIRE(data.size() == msgLen);
        memcpy(getMsg, DEQUE_DATA(data), msgLen);
        REQUIRE(msg.compare(getMsg) == 0);
        server.send(protocol, cid, data);
        cpn::Utils::msleep(1);

        data = client.receive(protocol, msgLen);
        memcpy(getMsg, DEQUE_DATA(data), msgLen);
        REQUIRE(data.size() == msgLen);
        REQUIRE(msg.compare(getMsg) == 0);
        REQUIRE(client.send(protocol, data, msgLen) == msgLen);
        cpn::Utils::msleep(1);

        clientList = server.getConnectionList();
        cid = clientList[0];
        data = server.receive(protocol, cid, msgLen);
        REQUIRE(data.size() == msgLen);
        memcpy(getMsg, DEQUE_DATA(data), msgLen);
        REQUIRE(msg.compare(getMsg) == 0);
        REQUIRE(server.send(protocol, cid, data) == data.size());
        cpn::Utils::msleep(1);

        data = client.receive(protocol, msgLen);
        memset(getMsg, 0, msgLen);
        memcpy(getMsg, DEQUE_DATA(data), data.size());
        REQUIRE(data.size() == msgLen);
        REQUIRE(msg.compare(getMsg) == 0);
        REQUIRE(client.send(protocol, data, msgLen) == msgLen);
        cpn::Utils::msleep(1);


        clientList = server.getConnectionList();
        cid = clientList[0];
        data = server.receive(protocol, cid, msgLen);
        REQUIRE(data.size() == msgLen);
        memcpy(getMsg, DEQUE_DATA(data), msgLen);
        REQUIRE(msg.compare(getMsg) == 0);
        REQUIRE(server.send(protocol, cid, data) == data.size());
        cpn::Utils::msleep(1);

        data = client.receive(protocol, 1);
        REQUIRE(data.size() == 1);
        REQUIRE(data[0] == 'H');
        protocol = CPN_UDP;
    }

    server.stop();
}

TEST_CASE( "Successive connection disconnection", "[CrossPlatformNetwork]" ) {
    uint16_t port = rngRand.getRandPort();
    std::string ip = "127.0.0.1";

    cpn::Client client;
    cpn::Server server;

    REQUIRE( server.start(port) );

    for (size_t i = 0; i < 10; ++i) {
        REQUIRE( client.connectTo(ip, port) );
        REQUIRE( client.disconnect() );
    }

    server.stop();
}
