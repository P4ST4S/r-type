/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** INetwork
*/

#pragma once

#include "CpnUtils.hpp"

#include <cstdint>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <deque>

namespace cpn
{
    class INetwork
    {
    public:
        virtual ~INetwork() = default;




        // INITIAL

        /**
         ** @brief Starts a thread that will handle all data reception
         ** and store them in multiple buffer based on sender ip:port.
         ** Should not be manually used
         **
         ** @return true if the thread starts successfully, false if already running.
         */
        virtual bool startReception(void) noexcept = 0;

        /**
         ** @brief Stop the reception thread. Should not be manually used
         **
         ** @return true if the thread stops successfully, false if already stopped.
         */
        virtual bool stopReception(void) noexcept = 0;




        // CONNECTION MANAGEMENT

        /**
         ** @brief Remove connection from the server as if it never connected.
         **
         ** @param cid The connection you want to remove.
         ** @return true Removal was successful. false The connection doesn't exist.
         */
        virtual bool removeConnection(cpn::connectionID_t cid) noexcept = 0;

        /**
         ** @brief Remove connection from the server as if it never connected.
         **
         ** @param ip
         ** @param port
         ** @return true Removal was successful. false The connection doesn't exist.
         */
        virtual bool removeConnection(const std::string &ip, uint16_t port) noexcept = 0;

        /**
         ** @brief Remove all connection from the server as if it never connected.
         **
         ** @param ip
         ** @param port
         ** @return true Removal was successful. false The connection doesn't exist.
         */
        virtual void removeAllConnection(void) noexcept = 0;

        /**
         ** @brief Remove connection flagged as disconnected from the server as if it never connected.
         **
         ** @param ip
         ** @param port
         ** @return true Removal was successful. false The connection doesn't exist.
         */
        virtual void removeAllDisconnectedConnection(void) noexcept = 0;


        /**
         ** @brief Register a new connection to the server.
         **
         ** @param sockAddr The sockaddr_in of the connection.
         ** @param fd The file descriptor of the connection (TCP).
         ** @return true
         ** @return false
         */
        virtual bool addConnection(const struct sockaddr_in &sockAddr, int fd) noexcept = 0;




        // DATA MANAGEMENT

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
        virtual void writeInConnectionBuffer(int protocol, cpn::connectionID_t cid, const std::deque<uint8_t> &data) noexcept = 0;

        /**
         ** @brief Clear the connection buffer.
         **
         ** @param cid The connection you want to clear the buffer.
         ** @return true The buffer was cleared. false The buffer doesn't exist.
         */
        virtual bool clearConnectionBuffer(int protocol, cpn::connectionID_t cid) noexcept = 0;




        // GETTERS

        /**
         ** @brief The IP of this server.
         **
         ** @param protocol If you want to TCP or UDP,
         ** return UDP by default or when protocol is invalid.
         ** @return std::string
         */
        virtual std::string getIP(int protocol = CPN_UDP) const = 0;

        /**
         ** @brief The IP of this server.
         **
         ** @param protocol If you want to TCP or UDP,
         ** return UDP by default or when protocol is invalid.
         ** @return uint32_t
         */
        virtual uint32_t getIPInt(int protocol = CPN_UDP) const = 0;

        /**
         ** @brief Get the Socket object
         **
         ** @param protocol If you want to TCP or UDP,
         ** return UDP by default or when protocol is invalid.
         ** @return int
         */
        virtual int getSocket(int protocol = CPN_UDP) const = 0;

        /**
         ** @brief Get the Sockaddr object
         **
         ** @param protocol If you want to TCP or UDP,
         ** return UDP by default or when protocol is invalid.
         ** @return const sockaddr_in&
         */
        virtual const sockaddr_in &getSockaddr(int protocol = CPN_UDP) const = 0;

        /**
         ** @brief The port of this server.
         **
         ** @return uint16_t
         */
        virtual uint16_t getPort(void) const = 0;

        /**
         ** @brief The connection ID of this server.
         **
         ** @return uint16_t
         */
        virtual cpn::connectionID_t getConnectionId(void) const = 0;

        /**
         ** @brief Indicates if the reception thread is running.
         **
         ** @return true
         ** @return false
         */
        virtual bool isReceptionThreadRunning(void) const noexcept = 0;

        /**
         ** @brief Get the max connection possible connections
         ** which after what the server will ignore any new connection.
         **
         **
         ** @return The max number of connection connection.
         ** If got -1 the server will no longer have connection limit and accept every connection.
         */
        virtual ssize_t getMaxConnection(void) const noexcept = 0;

        /**
         ** @brief The number of actual connection connected.
         **
         ** @return The number of actual connection connected.
         */
        virtual size_t getConnectionCount(void) const noexcept = 0;

        /**
         ** @brief Indicate if the server still have connection slot available.
         **
         ** @return true If the server still have connection slot available. false otherwise.
         */
        virtual bool connectionSlotAvailable(void) const noexcept = 0;

        /**
         ** @brief Returns an std::vector of connectionID_t of every connection connected to the server.
         **
         ** @return std::vector<cpn::connectionID_t>
         */
        virtual std::deque<cpn::connectionID_t> getConnectionList(void) noexcept = 0;

        /**
         ** @brief Give you access to connection cid, buffer and mutex.
         ** Since this structure is actively modified by the server,
         ** you must mutex lock when doing modification.
         ** In some case the connection might no longer exist to avoid
         ** issue make sure to check that connectionData.cid.valid == 1.
         **
         ** @param cid The connection you want to get the information.
         ** @return connectionData_t&
         */
        virtual cpn::connectionData_t &getConnection(cpn::connectionID_t cid) = 0;

        /**
         ** @brief Give you access to connection cid, buffer and mutex.
         ** Since this structure is actively modified by the server,
         ** you must mutex lock when doing modification.
         ** In some case the connection might no longer exist to avoid
         ** issue make sure to check that connectionData.cid.valid == 1.
         **
         ** @param cid The connection you want to get the information.
         ** @return connectionData_t&
         */
        virtual const cpn::connectionData_t &getConnection(cpn::connectionID_t cid) const = 0;

        /**
         ** @brief Let you know if the serv knows this cid.
         **
         ** @param cid The connection you want to get the information.
         ** @return bool true if the server knows this cid. false otherwise.
         */
        virtual bool connectionExist(cpn::connectionID_t cid) = 0;

        /**
         ** @brief Gets the connections buffer size.
         **
         ** @param cid The connection you want to get the buffer size.
         ** @return connectionData_t& The size of the buffer (can be 0) or -1 if the buffer doesn't exist.
         */
        virtual ssize_t getConnectionBufferSize(int protocol, cpn::connectionID_t cid) noexcept = 0;

        /**
         ** @brief Give all the struct pollfd corresponding to the connected clients
         **
         ** @return std::vector<struct pollfd>&
         */
        virtual std::vector<struct pollfd> &generatePollFds(void) noexcept = 0;

        /**
         ** @brief Get the Connection Id object
         **
         ** @param fd
         ** @return cpn::connectionID_t
         */
        virtual cpn::connectionID_t getFdConnectionId(int fd) = 0;


        virtual const std::string &getClassName(void) const = 0;

    protected:
        #ifdef _WIN32
            virtual bool initWsa(void) = 0;
            virtual void stopWsa(void) = 0;
        #endif



        virtual ssize_t genericSend(int protocol, cpn::connectionID_t cid, const std::deque<uint8_t> &buffer, size_t size) const = 0;

        virtual std::deque<uint8_t> genericReceive(int protocol, cpn::connectionID_t cid, size_t size) = 0;

        virtual std::deque<uint8_t> genericReceiveFromAny(int protocol, cpn::connectionID_t &cid, size_t size) = 0;
    private:
    };
}
