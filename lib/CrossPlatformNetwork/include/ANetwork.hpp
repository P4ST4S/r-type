/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** ANetwork
*/

#pragma once

#include "INetwork.hpp"

namespace cpn {
    class ANetwork : public cpn::INetwork {
    public:
        ANetwork();
        ~ANetwork();




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
         ** @brief Stop the reception thread. Should not be manually used.
         **
         ** @return true if the thread stops successfully, false if already stopped.
         */
        bool stopReception(void) noexcept;




        // CONNECTION MANAGEMENT

        /**
         ** @brief Remove connection from the server as if it never connected.
         **
         ** @param cid The connection you want to remove.
         ** @return true Removal was successful. false The connection doesn't exist.
         */
        bool removeConnection(cpn::connectionID_t cid) noexcept;

        /**
         ** @brief Remove connection from the server as if it never connected.
         **
         ** @param ip
         ** @param port
         ** @return true Removal was successful. false The connection doesn't exist.
         */
        bool removeConnection(const std::string &ip, uint16_t port) noexcept;

        /**
         ** @brief Remove all connection from the server as if it never connected.
         **
         ** @param ip
         ** @param port
         ** @return true Removal was successful. false The connection doesn't exist.
         */
        void removeAllConnection(void) noexcept;

        /**
         ** @brief Remove connection flagged as disconnected from the server as if it never connected.
         **
         ** @param ip
         ** @param port
         ** @return true Removal was successful. false The connection doesn't exist.
         */
        void removeAllDisconnectedConnection(void) noexcept;

        /**
         ** @brief Register a new connection to the server.
         **
         ** @param sockAddr The sockaddr_in of the connection.
         ** @param fd The file descriptor of the connection (TCP).
         ** @return true
         ** @return false
         */
        bool addConnection(const struct sockaddr_in &sockAddr, int fd) noexcept;




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
        bool clearConnectionBuffer(int protocol, cpn::connectionID_t cid) noexcept;





        // GETTERS

        /**
         ** @brief The IP of this server.
         **
         ** @param protocol If you want to TCP or UDP,
         ** return UDP by default or when protocol is invalid.
         ** @return std::string
         */
        std::string getIP(int protocol = CPN_UDP) const;

        /**
         ** @brief The IP of this server.
         **
         ** @param protocol If you want to TCP or UDP,
         ** return UDP by default or when protocol is invalid.
         ** @return uint32_t
         */
        uint32_t getIPInt(int protocol = CPN_UDP) const;

        /**
         ** @brief Get the Socket object
         **
         ** @param protocol If you want to TCP or UDP,
         ** return UDP by default or when protocol is invalid.
         ** @return int
         */
        int getSocket(int protocol = CPN_UDP) const;

        /**
         ** @brief Get the Sockaddr object
         **
         ** @param protocol If you want to TCP or UDP,
         ** return UDP by default or when protocol is invalid.
         ** @return const sockaddr_in&
         */
        const sockaddr_in &getSockaddr(int protocol = CPN_UDP) const;

        /**
         ** @brief The port of this server.
         **
         ** @return uint16_t
         */
        uint16_t getPort(void) const;

        /**
         ** @brief The connection ID of this server.
         **
         ** @return uint16_t
         */
        cpn::connectionID_t getConnectionId(void) const;

        /**
         ** @brief Indicates if the reception thread is running.
         **
         ** @return true
         ** @return false
         */
        bool isReceptionThreadRunning(void) const noexcept;

        /**
         ** @brief Get the max connection possible connections
         ** which after what the server will ignore any new connection.
         **
         **
         ** @return The max number of connection connection.
         ** If got -1 the server will no longer have connection limit and accept every connection.
         */
        ssize_t getMaxConnection(void) const noexcept;

        /**
         ** @brief The number of actual connection connected.
         **
         ** @return The number of actual connection connected.
         */
        size_t getConnectionCount(void) const noexcept;

        /**
         ** @brief Indicate if the server still have connection slot available.
         **
         ** @return true If the server still have connection slot available. false otherwise.
         */
        bool connectionSlotAvailable(void) const noexcept;

        /**
         ** @brief Returns an std::vector of connectionID_t of every connection connected to the server.
         **
         ** @return std::vector<cpn::connectionID_t>
         */
        std::deque<cpn::connectionID_t> getConnectionList(void) noexcept;

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
        cpn::connectionData_t &getConnection(cpn::connectionID_t cid);

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
        const cpn::connectionData_t &getConnection(cpn::connectionID_t cid) const;

        /**
         ** @brief Let you know if the serv knows this cid.
         **
         ** @param cid The connection you want to get the information.
         ** @return bool true if the server knows this cid. false otherwise.
         */
        bool connectionExist(cpn::connectionID_t cid);

        /**
         ** @brief Gets the connections buffer size.
         **
         ** @param cid The connection you want to get the buffer size.
         ** @return connectionData_t& The size of the buffer (can be 0) or -1 if the buffer doesn't exist.
         */
        ssize_t getConnectionBufferSize(int protocol, cpn::connectionID_t cid) noexcept;

        /**
         ** @brief Give all the struct pollfd corresponding to the connected clients
         **
         ** @return std::vector<struct pollfd>&
         */
        std::vector<struct pollfd> &generatePollFds(void) noexcept;

        /**
         ** @brief Get the Connection Id object
         **
         ** @param fd
         ** @return cpn::connectionID_t
         */
        cpn::connectionID_t getFdConnectionId(int fd);

        const std::string &getClassName(void) const;

    protected:
        #ifdef _WIN32
            bool _wsaRunning = false;
            WSADATA _wsa; /**< Windows-specific data for socket initialization. */
            bool initWsa(void);
            void stopWsa(void);
        #endif

        ssize_t genericSend(int protocol, cpn::connectionID_t cid, const std::deque<uint8_t> &buffer, size_t size) const;

        std::deque<uint8_t> genericReceive(int protocol, cpn::connectionID_t cid, size_t size);

        std::deque<uint8_t> genericReceiveFromAny(int protocol, cpn::connectionID_t &cid, size_t size);

        int _tcpSock = -1;
        int _udpSock = -1;
        sockaddr_in _tcpSockaddr;
        sockaddr_in _udpSockaddr;
        int _port = -1; /**< The port number on which the server is running. */

        bool _dataIsSet = false;
        bool _socketRunning = false;
        bool _recvRunning = false;

        ssize_t _maxConnection = 1;
        std::unordered_map<uint64_t, connectionData_t> _connections;
        std::unordered_map<int, std::pair<struct pollfd, cpn::connectionID_t>> _connectionFds;
        std::vector<struct pollfd> _pollFds;

        std::thread _udpRecvThread;
        std::thread _tcpRecvThread;
        std::mutex _connectionMut;
        std::mutex _udpConnectionMut;
        std::mutex _tcpConnectionMut;

        std::string _loggersName = "ANETWORK";
    private:
    };
}
