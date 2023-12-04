/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** CrossPlatformClient
*/

#pragma once

#include "CpnUtils.hpp"
#include "ANetwork.hpp"
#include "ReceptionFunctions.hpp"

#include <iostream>
#include <stdint.h>
#include <string.h>

#define CLI_LOG_NAME "CLIENT"

namespace cpn {

    /**
     ** @brief This class is made to simplify udp client usage.
     ** It let you set ip:port and the class will
     ** only send and receive package from it.
     ** Every package received from another will be ignored.
     */
    class Client : public cpn::ANetwork {
    public:
        Client();
        /**
         ** @brief Construct a new Cross Platform Client object and set ip:port.
         **
         ** @param ip The ip of the targeted device.
         ** @param port The port of the targeted device.
         */
        Client(const std::string& ip, uint16_t port);
        ~Client();

        void writeInConnectionBuffer(int protocol, cpn::connectionID_t cid, const std::deque<uint8_t> &data) noexcept;

        void clearConnection(void) noexcept;

        /**
         ** @brief Set the ip and the port to send and receive data.
         **
         ** @param ip The ip of the targeted device.
         ** @param port The port of the targeted device.
         */
        bool connectTo(const std::string& ip, uint16_t port) noexcept;

        /**
         ** @brief Check if the client is connected to something.
         **
         ** @return true if connected. false if not.
         */
        bool isConnected(void) const noexcept;

        /**
         ** @brief End the connection the currently established connection
         ** do nothing if not connected.
         **
         ** @return true
         ** @return false
         */
        bool disconnect(void) noexcept;

        /**
         ** @brief Check if the ip and the port are set.
         **
         ** @return true
         ** @return false
         */
        bool isAddressSet(void) const noexcept;

        /**
         ** @brief Starts a thread that will handle all data reception.
         **
         ** @return true if the thread starts successfully, false if already running.
         */
        bool startReception(void) noexcept;


        /**
         ** @brief Send data to the address set with setAddress() or in the constructor.
         **
         ** @param data The data you want to send.
         ** @param size The size of the data you want to send.
         ** @return ssize_t <0 means there were an error.
         */
        ssize_t send(int protocol, const std::deque<uint8_t> &data, size_t size) const;

        /**
         ** @brief Send data to the address set with setAddress() or in the constructor.
         **
         ** @param data The data you want to send.
         ** @return ssize_t <0 means there were an error.
         */
        ssize_t send(int protocol, const std::deque<uint8_t> &data) const;

        /**
         ** @brief Receive up to {size} byte of data from
         ** the address gived in setAddress() or in the constructor.
         ** Everything coming from another address will be ignored.
         **
         ** @param size The size of the data you want to read.
         ** @return std::deque<uint8_t>
         */
        std::deque<uint8_t> receive(int protocol, size_t size);

        /**
         ** @brief Receive all the data forom
         ** the address gived in setAddress() or in the constructor.
         ** Everything coming from another address will be ignored.
         **
         ** @return std::deque<uint8_t>
         */
        std::deque<uint8_t> receive(int protocol);

        /**
         ** @brief The IP set with setAddress() or in the constructor.
         **
         ** @param protocol If you want to TCP or UDP,
         ** return UDP by default or when protocol is invalid.
         ** @return std::string
         */
        std::string getTargetIP(int protocol = CPN_UDP) const;

        /**
         ** @brief The IP set with setAddress() or in the constructor.
         **
         ** @param protocol If you want to TCP or UDP,
         ** return UDP by default or when protocol is invalid.
         ** @return std::string
         */
        uint32_t getTargetIPInt(int protocol = CPN_UDP) const;

        /**
         ** @brief The port set with setAddress() or in the constructor.
         **
         ** @return uint16_t
         */
        uint16_t getTargetPort(void) const;

        /**
         ** @brief The connection ID generated with setAddress() or in the constructor.
         **
         ** @return cpn::connectionID_t
         */
        cpn::connectionID_t getTargetConnectionID(void) const;

    private:
        void throwWhenAddressNotSet(void) const;

        bool initTcpConnection(const std::string& ip, uint16_t port) noexcept;
        bool initUdpConnection(const std::string& ip, uint16_t port) noexcept;

        cpn::connectionID_t _targetCid;

    };
}
