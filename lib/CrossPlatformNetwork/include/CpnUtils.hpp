/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** CrossPlatformNetworkStruct
*/

#pragma once

#include "Buffer.hpp"
#ifdef _WIN32
    namespace win {
        #include <winsock2.h>
        #include <ws2tcpip.h>
        #pragma comment(lib, "ws2_32.lib")
    }
    // typedef win::sockaddr_in sockaddr_in;
    // typedef win::DWORD DWORD;

    // #define IPPROTO_UDP 17
    using namespace win;
#else
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <unistd.h>
    #include <poll.h>
#endif
#include <iostream>
#include <cstdint>
#include <string>
#include <cstring>
#include <vector>
#include <mutex>
#include <thread>

// #define CPN_LOG

namespace cpn {
    #define CPN_TCP (1 << 0)
    #define CPN_UDP (1 << 1)
    #define MAX_BUF_SIZE 1024 /* Maximum buffer size for receiving data. */
    #define LOCALHOST "127.0.0.1"
    #define CID_VALID(cid) (cid._valid == 1)

    typedef union conectionID_u {
        uint64_t _id;
        struct {
            uint32_t _ip;
            uint16_t _port;
            /*
            1 if valid 0 if not.
            Useful only when using getClient() to check
            if the client exist.
            */
            uint16_t _valid;
        };
        uint8_t _bytes[8];

        conectionID_u(uint64_t __id__) : _id(__id__) {}
        conectionID_u() : _id(0) {}

        operator uint64_t() const {
            return this->_id;
        }
    } connectionID_t;

    #ifdef CPN_LOG
        #define LOG(writer, msg) std::cout << "[" << writer << "] " << msg << std::endl
    #else
        #define LOG(writer, msg)
    #endif

    typedef struct connectionData_s {
        const cpn::connectionID_t cid;
        bool connected;
        int fd;
        cpn::Buffer udpBuf;
        cpn::Buffer tcpBuf;
        std::time_t lastActivity; // Let you what time (in seconds) last package has been sent

        connectionData_s(cpn::connectionID_t _cid) : cid(_cid) {}
        connectionData_s() : cid() {}
    } connectionData_t;

    class Utils {
    public:
        /**
         ** @brief Convert a readable string ip address to an uint32_t.
         **
         ** @param ip
         ** @return uint32_t
         */
        static uint32_t ipStringToInt(const std::string &ip);

        /**
         ** @brief Convert an uint32_t ip address to a readable string.
         **
         ** @param ip
         ** @return std::string
         */
        static std::string ipIntToString(uint32_t ip);



        /**
         ** @brief Extract sockaddr_in information and create the according connectionID_t.
         **
         ** @param sockaddr
         ** @return cpn::connectionID_t
         */
        static cpn::connectionID_t getConnectionID(const sockaddr_in &sockaddr);
        /**
         ** @brief Create a connectionID_t from an ip and a port.
         **
         ** @param ip
         ** @param port
         ** @return cpn::connectionID_t
         */
        static cpn::connectionID_t getConnectionID(const std::string &ip, uint16_t port);
        /**
         ** @brief Create a connectionID_t from an ip and a port.
         **
         ** @param ip
         ** @param port
         ** @return cpn::connectionID_t
         */
        static cpn::connectionID_t getConnectionID(uint32_t ip, uint16_t port);


        /**
         ** @brief Get the string ip of a sockaddr_in struct.
         **
         ** @param sockaddr
         ** @return std::string
         */
        static std::string getIP(const sockaddr_in &sockaddr);
        /**
         ** @brief Get the string ip of a connectionID_t.
         **
         ** @param cid
         ** @return std::string
         */
        static std::string getIP(cpn::connectionID_t cid);
        /**
         ** @brief Get the ip of a sockaddr_in struct.
         **
         ** @param sockaddr
         ** @return uint32_t
         */
        static uint32_t getIPInt(const sockaddr_in &sockaddr);
        /**
         ** @brief Get the ip of a connectionID_t.
         **
         ** @param cid
         ** @return uint32_t
         */
        static uint32_t getIPInt(cpn::connectionID_t cid);
        /**
         ** @brief Get the port of a sockaddr_in struct.
         **
         ** @param sockaddr
         ** @return uint16_t
         */
        static uint16_t getPort(const sockaddr_in &sockaddr);
        /**
         ** @brief Get the port of a connectionID_t.
         **
         ** @param cid
         ** @return uint16_t
         */
        static uint16_t getPort(cpn::connectionID_t cid);


        /**
         ** @brief Set the sockaddr_in struct with the connectionID_t information.
         **
         ** @param sockaddr
         ** @param cid
         */
        static void setSockAddr(sockaddr_in &sockaddr, const cpn::connectionID_t &cid);
        /**
         ** @brief Set the sockaddr_in struct with the ip and port information.
         **
         ** @param sockaddr
         ** @param ip
         ** @param port
         */
        static void setSockAddr(sockaddr_in &sockaddr, const std::string &ip, uint16_t port);
        /**
         ** @brief Set the sockaddr_in struct with the ip and port information.
         **
         ** @param sockaddr
         ** @param ip
         ** @param port
         */
        static void setSockAddr(sockaddr_in &sockaddr, uint32_t ip, uint16_t port);


        /**
         ** @brief Create an udp socket.
         ** Function is cross-platform.
         **
         ** @return int Same return value as socket function. (Can vary depending the OS)
         */
        static int udpSocket(void);
        /**
         ** @brief Create a tcp socket.
         ** Function is cross-platform.
         **
         ** @return int Same return value as socket function. (Can vary depending the OS)
         */
        static int tcpSocket(void);
        /**
         ** @brief Returns true if socket creation succeed.
         ** Function is cross-platform.
         **
         ** @param sock
         ** @return true
         ** @return false
         */
        static bool socketCreationSuccess(int socket);
        /**
         ** @brief Bind a socket to a sockaddr_in struct.
         **
         ** @param socket
         ** @param sockaddr
         ** @return int Return the binded port or -1 if error.
         */
        static int bindSocket(int socket, const sockaddr_in &sockaddr);
        /**
         ** @brief Bind a socket to a sockaddr_in struct.
         **
         ** @param socket
         ** @param sockaddr
         ** @return int Return the binded port or -1 if error.
         */
        static bool bindSucess(int bindReturn);
        /**
         ** @brief Close socket.
         ** Function is cross-platform.
         ** @param socket
         */
        static void closeSocket(int socket);

        /**
         ** @brief Work similarly to recvfrom but return a deque of uint8_t.
         ** Function is cross-platform.
         **
         ** @param socket
         ** @param buf The buffer you want to receive in.
         ** @param size How long you want to write in the buffer.
         ** @param servaddr A valid struct sockaddr_in
         ** @param nonBlock On true doesn't wait for a package.
         ** @return ssize_t
         */
        static std::deque<uint8_t> recvFrom(int socket, size_t size, struct sockaddr_in &servaddr, bool nonBlock = true);
        /**
         ** @brief Work similarly to recv but return a deque of uint8_t.
         ** Function is cross-platform.
         **
         ** @param socket
         ** @param buf The buffer you want to receive in.
         ** @param size How long you want to write in the buffer.
         ** @param nonBlock On true doesn't wait for a package.
         ** @return ssize_t
         */
        static std::deque<uint8_t> recvFromSocket(int socket, size_t size, bool nonBlock = true);
        /**
         ** @brief Create a temporary socket to send a package.
         ** Should not be used to commonly send package.
         ** Function is cross-platform.
         **
         ** @param ip IP to send the package
         ** @param port Port to send the package
         ** @param buf Bytes you want to send
         ** @param size buf size
         ** @return ssize_t the size sent or -1 on failure.
         */
        static ssize_t sendToUdp(const std::string &ip, uint16_t port, const void *buf, size_t size);
        /**
         ** @brief Create a temporary socket to send a package.
         ** Should not be used to commonly send package.
         ** Function is cross-platform.
         **
         ** @param ip IP to send the package
         ** @param port Port to send the package
         ** @param buf Bytes you want to send
         ** @param size buf size
         ** @return ssize_t the size sent or -1 on failure.
         */
        static ssize_t sendToTcp(const std::string &ip, uint16_t port, const void *buf, size_t size);
        /**
         ** @brief Send data to the specified combo socket/sockaddr in TCP or UDP.
         ** You should use this function to send data.
         ** Function is cross-platform.
         **
         ** @param socket
         ** @param sockaddr
         ** @param data
         ** @param size
         ** @return ssize_t
         */
        static ssize_t sendTo(int socket, const sockaddr_in &sockaddr, const void *data, size_t size);
        /**
         ** @brief Send data to the specified socket.
         **
         ** @param socket
         ** @param data
         ** @param size
         ** @return ssize_t
         */
        static ssize_t sendToSocket(int socket, const void *data, size_t size);

        /**
         ** @brief Wait the specified time.
         ** Function is cross-platform.
         **
         ** @param seconds
         */
        static void sleep(size_t seconds);
        /**
         ** @brief Wait the specified time.
         ** Function is cross-platform.
         **
         ** @param milliSeconds
         */
        static void msleep(size_t milliSeconds);
        /**
         ** @brief Wait the specified time.
         ** Function is cross-platform.
         **
         ** @param microSeconds
         */
        static void usleep(size_t microSeconds);
        /**
         ** @brief Wait the specified time.
         ** Function is cross-platform.
         **
         ** @param nanoSeconds
         */
        static void nsleep(size_t nanoSeconds);
    };
}

std::ostream &operator<<(std::ostream &os, const cpn::connectionID_t &cid);
std::ostream &operator<<(std::ostream &os, const sockaddr_in &sockaddr);
