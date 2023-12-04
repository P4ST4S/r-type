/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** Connection
*/

#pragma once

#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <ctime>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

class Connection {
    public:
        Connection(int port);
        ~Connection();

        std::string receive();
        void send(const std::string& msg, const std::string& clientIp, int clientPort);

    private:
        int serverSocket;
        int serverPort;
        struct sockaddr_in serverAddr;
        static const int MAX_BUFFER_SIZE = 1024;
};
