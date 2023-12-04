/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** BufferClassTest
*/

#include "tests.hpp"
#include "CrossPlatformNetwork.hpp"

bool cmpBytes(const std::string &expect, const std::deque<uint8_t> &actual)
{
    return (memcmp(expect.c_str(), &(*(actual.begin())), actual.size()) == 0);
}

TEST_CASE( "Buffer Class write test", "[Buffer Class]" ) {

    cpn::Buffer buffer;
    std::deque<uint8_t> data = {'0', '1', '2', '3', '4', '5', '6', '7'};

    REQUIRE(buffer.writeIn("Hello World!", 12) == 12);
    REQUIRE(cmpBytes("Hello World!", buffer.get()));
    REQUIRE(buffer.writeIn("Hello World!", 5) == 5);
    REQUIRE(cmpBytes("Hello World!Hello", buffer.get()));
    REQUIRE(buffer.writeIn("Hello World!", 0) == 0);
    REQUIRE(cmpBytes("Hello World!Hello", buffer.get()));

    REQUIRE(buffer.writeIn(data, 8) == 8);
    REQUIRE(cmpBytes("Hello World!Hello01234567", buffer.get()));
    REQUIRE(buffer.writeIn(data, 2) == 2);
    REQUIRE(cmpBytes("Hello World!Hello0123456701", buffer.get()));
    REQUIRE(buffer.writeIn(data, 0) == 0);
    REQUIRE(cmpBytes("Hello World!Hello0123456701", buffer.get()));
    REQUIRE(buffer.writeIn(data) == 8);
    REQUIRE(cmpBytes("Hello World!Hello012345670101234567", buffer.get()));
}

TEST_CASE( "Buffer Class read test", "[Buffer Class]" ) {

    cpn::Buffer buffer;
    std::deque<uint8_t> data;

    REQUIRE(buffer.writeIn("Hello World!Hello012345670101234567", 35) == 35);

    data = buffer.readIn(12);
    REQUIRE(data.size() == 12);
    REQUIRE(cmpBytes("Hello World!", data));
    data = buffer.readIn(0);
    REQUIRE(data.size() == 0);
    REQUIRE(cmpBytes("", data));
    data = buffer.readIn(100);
    REQUIRE(data.size() == 23);
    REQUIRE(cmpBytes("Hello012345670101234567", data));
    data = buffer.readIn(100);
    REQUIRE(data.size() == 0);
    REQUIRE(cmpBytes("", data));

}

TEST_CASE( "Buffer Class buffer modification test", "[Buffer Class]" ) {

    cpn::Buffer buffer;
    std::deque<uint8_t> data;

    REQUIRE(buffer.writeIn("Hello World!", 12) == 12);

    REQUIRE(buffer.size() == 12);
    buffer.get()[1] = 'a';
    REQUIRE(cmpBytes("Hallo World!", buffer.get()));
    data = buffer.readIn(12);
    REQUIRE(cmpBytes("Hallo World!", data));
}
