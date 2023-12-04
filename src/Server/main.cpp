/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** main
*/

#include "RTypeServer.hpp"

#include <iostream>
#include <stdint.h>
#include <string.h>

static int getPort(int argc, char **argv)
{
    int port;
    char *decodedPart = nullptr;

    if (argc < 2) {
        std::cerr << "Port parameter is missing" << std::endl;
        return (-1);
    }
    port = strtol(argv[1], &decodedPart, 10);
    if (port < 0 || port > UINT16_MAX || decodedPart[0] == argv[1][0]) {
        std::cerr << argv[1] << " is not a valid port" << std::endl;
        return (-1);
    }
    return (port);
}

int main(int argc, char **argv)
{
    RTypeServer server;
    int port = getPort(argc, argv);
    srand(std::time(nullptr));

    if (port < 0) {
        std::cerr << "USAGE: " << argv[0] << " [port]" << std::endl;
        return (84);
    }
    server.run(port);
    return (0);
}
