/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** CrossPlatformServer
*/

#pragma once

#include <mutex>

#include "CpnUtils.hpp"
#include "Buffer.hpp"
#include "ANetwork.hpp"
#include "ReceptionFunctions.hpp"

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <deque>
#include <thread>
#include <chrono>
#include <ctime>
#include <unordered_map>

#define SRV_LOG_NAME "SERVER"

namespace cpn {
    /**
     * @brief The CrossPlatformServer class manages a UDP server for cross-platform networking.
     *
     * This class provides functionality for starting, stopping, receiving, and sending UDP data packets.
     * It can handle variable-sized data packets and supports both blocking and non-blocking modes.
     */
    class Server : public cpn::ANetwork {
    public:
        // INITIAL

        /**
         * @brief Construct a new CrossPlatformServer object.
         */
        Server();

        /**
         * @brief Destroy the CrossPlatformServer object.
         */
        ~Server();

        /**
         * @brief Start the UDP server on the specified port.
         *
         * @param port The port on which to start the server.
         * @return true if the server starts successfully, false otherwise.
         */
        bool start(uint16_t port = 0) noexcept;

        /**
         * @brief Stop the UDP server.
         *
         * @return true if the server stops successfully, false otherwise.
         */
        bool stop() noexcept;

        /**
         ** @brief Starts a thread that will handle all data reception
         ** and store them in multiple buffer based on sender ip:port.
         **
         ** @return true if the thread starts successfully, false if already running.
         */
        bool startReception(void) noexcept;





        // CONNECTION MANAGEMENT

        /**
         ** @brief Server can connect to another server.
         ** Even if the server is locked or full.
         **
         ** @param ip
         ** @param port
         ** @return True if the connection was added, false if the connection already exist.
         */
        bool connectTo(const std::string &ip, uint16_t port) noexcept;





        // DATA MANAGEMENT

        /**
         ** @brief Return up to {size} byte in a std::deque<uint8_t>.
         ** Returns an empty vector if the cid doesn't exist or if protocol is invalid.
         **
         ** @param protocol If you want to receive either TCP or UDP receieved data.
         ** @param cid Connection you want to receive from.
         ** @param size The quantity of bytes you want to receive
         ** @return std::vector<uint8_t>
         */
        std::deque<uint8_t> receive(int protocol, connectionID_t cid, size_t size) noexcept;

        /**
         ** @brief Return all the data in the buffer of the connection.
         ** Returns an empty vector if the cid doesn't exist or if protocol is invalid.
         **
         ** @param protocol If you want to receive either TCP or UDP receieved data.
         ** @param cid Connection you want to receive from.
         ** @return std::deque<uint8_t>
         */
        std::deque<uint8_t> receive(int protocol, connectionID_t cid) noexcept;

        /**
         ** @brief Return up to {size} byte in a std::deque<uint8_t>
         ** from the first connection that is found to have data
         ** of the corresponding protocol.
         ** You will receive nothing if no connection has data or if protocol is invalid.
         **
         ** @param cid Connection you received from or an invalid cid if nothing was received.
         ** @param size Bytes you want to receive
         ** @return std::vector<uint8_t>
         */
        std::deque<uint8_t> receiveFromAny(int protocol, connectionID_t &cid, size_t size) noexcept;

        /**
         ** @brief Return all the data in the buffer of the first connection
         ** that is found to have data of the corresponding protocol.
         ** You will receive nothing if no connection has data or if protocol is invalid.
         **
         ** @param cid Connection you received from or an invalid cid if nothing was received.
         ** @return std::deque<uint8_t>
         */
        std::deque<uint8_t> receiveFromAny(int protocol, connectionID_t &cid) noexcept;

        /**
         * @brief Send data to a specific connection.
         *
         * @param protocol If you want to send in TCP or UDP or both at the same time.
         * @param cid The adress of the receiver.
         * @param buffer The data to send.
         * @param size The total size of data to send.
         * @return The size of the sent data, or a negative value in case of failure.
         */
        ssize_t send(int protocol, connectionID_t cid, const std::deque<uint8_t> &buffer, size_t size) const noexcept;

        /**
         * @brief Send data to a specific connection.
         *
         * @param protocol If you want to send in TCP or UDP or both at the same time.
         * @param cid The adress of the receiver.
         * @param buffer The data to send.
         * @return The size of the sent data, or a negative value in case of failure.
         */
        ssize_t send(int protocol, connectionID_t cid, const std::deque<uint8_t> &buffer) const noexcept;

        /**
         * @brief Send data to a specific connection.
         *
         * @param protocol If you want to send in TCP or UDP or both at the same time.
         * @param cid The adresses of the receivers.
         * @param buffer The data to send.
         * @param size The total size of data to send.
         * @return The size of the sent data, or a negative value in case of failure.
         */
        std::deque<ssize_t> multiSend(int protocol, const std::deque<connectionID_t> &cid, const std::deque<uint8_t> &buffer, size_t size) const noexcept;

        /**
         * @brief Send data to a specific connection.
         *
         * @param protocol If you want to send in TCP or UDP or both at the same time.
         * @param cid The adresses of the receivers.
         * @param buffer The data to send.
         * @return The size of the sent data, or a negative value in case of failure.
         */
        std::deque<ssize_t> multiSend(int protocol, const std::deque<connectionID_t> &cid, const std::deque<uint8_t> &buffer) const noexcept;

        /**
         ** @brief This method should not be used, except by runReception() functions.
         ** It's purpose is to write into the temporary buffers of connection.
         ** However using by yourself it will bring data corruption,
         ** since this function is handled by the reception thread.
         **
         **
         ** @param cid The connection id
         ** @param data The data to write in the connection buffer.
         */
        void writeInConnectionBuffer(int protocol, cpn::connectionID_t cid, const std::deque<uint8_t> &data) noexcept;




        // SETTERS

        /**
         ** @brief If set on true the server will no longer accept any new connections
         ** and ignore any package from unknown connection.
         **
         ** @param lock
         */
        void setLock(bool lock) noexcept;

        /**
         ** @brief Set the max connection possible connections
         ** which after what the server will ignore any new connection.
         **
         **
         ** @param maxConnection (Default value: -1) Set the max number of connection connection.
         ** If set to -1 the server will no longer have connection limit and accept every connection.
         */
        void setMaxConnection(ssize_t maxConnection = -1) noexcept;

        /**
         ** @brief Will be sent to the connection removed from the server.
         **
         ** @param data
         */
        void dataToSendWhenRemove(std::deque<uint8_t> data) noexcept;

        /**
         ** @brief Will be sent to the connection when the server is locked.
         **
         ** @param data
         */
        void dataToSendWhenLock(std::deque<uint8_t> data) noexcept;

        /**
         ** @brief Will be sent to the connection when the server is full.
         **
         ** @param data
         */
        void dataToSendWhenMaxConnection(std::deque<uint8_t> data) noexcept;

        /**
         ** @brief Will be sent to the connection
         ** if trying to send data while not being connected via TCP.
         **
         ** @param data
         */
        void dataToSendWhenConnectionNotConnected(std::deque<uint8_t> data) noexcept;

        /**
         ** @brief Send the data when the connection is removed from the server.
         **
         ** @param data
         */
        void sendDataWhenRemove(int protocol, cpn::connectionID_t cid) noexcept;

        /**
         ** @brief Send the data to the connection when the server is locked.
         **
         ** @param data
         */
        void sendDataWhenLock(int protocol, cpn::connectionID_t cid) noexcept;

        /**
         ** @brief Send the data to the connection when the server is full.
         **
         ** @param data
         */
        void sendDataWhenMaxConnection(int protocol, cpn::connectionID_t cid) noexcept;

        /**
         ** @brief Send the data to the connection
         ** if trying to send data while not being connected via TCP.
         **
         ** @param data
         */
        void sendDataWhenConnectionNotConnected(int protocol, cpn::connectionID_t cid) noexcept;




        // GETTERS

        /**
         ** @brief Get lock status of the server
         **
         ** @param lock True the server no longer accept new connection,
         ** false every connection will be accepted as long as maxconnection is not reached.
         */
        bool getLock(void) const noexcept;



    private:
        bool bindPort(uint16_t port);

        bool initSockets(void);
        void closeSockets(void);


        bool _binded = false;

        std::deque<uint8_t> _dataWhenRemove;
        std::deque<uint8_t> _dataWhenLock;
        std::deque<uint8_t> _dataWhenMaxConnection;
        std::deque<uint8_t> _dataWhenConnectionNotConnected;

        bool _lock = false; /** block any new connection */
    };
}
