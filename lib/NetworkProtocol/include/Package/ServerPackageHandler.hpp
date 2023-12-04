/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** ServerPackageHandler
*/

#pragma once

#include "CrossPlatformNetwork.hpp"
#include "PackageFactory.hpp"
#include "PackageParser.hpp"

typedef struct ClientPackages_s {
    cpn::connectionData_t &_connectionData;
    PackageParser _udpPkgParser;
    PackageParser _tcpPkgParser;
    std::deque<Package> _pkgs;
    cpn::Buffer _udpBuffer;
    cpn::Buffer _tcpBuffer;

    ClientPackages_s(cpn::connectionData_t &connectionData, BodyFactory &bodyFactory) :
    _connectionData(connectionData), _udpPkgParser(bodyFactory), _tcpPkgParser(bodyFactory)
    {}

} ClientPackages_t;


class ServerPackageHandler {
public:
    ServerPackageHandler();
    ~ServerPackageHandler();

    /**
     ** @brief Start the server on the given port
     **
     ** @param port
     */
    bool start(uint16_t port) noexcept;

    /**
     ** @brief Stop the server
     **
     */
    bool stop(void) noexcept;

    /**
     ** @brief Remove connection from the server as if it never connected.
     **
     ** @param cid The connection you want to remove.
     ** @return true Removal was successful. false The connection doesn't exist.
     */
    bool removeConnection(cpn::connectionID_t cid) noexcept;

    /**
     ** @brief Set the maximum number of connection, default is -1 (no limit)
     ** If the limit is reached, the server will not accept new connections.
     **
     ** @param maxConnection
     */
    void setMaxConnection(ssize_t maxConnection = -1) noexcept;

    /**
     ** @brief Let you block any new connection when set to true.
     ** If true the server will not accept new connections.
     **
     ** @param lock
     */
    void setLock(bool lock) noexcept;

    /**
     ** @brief Get lock status of the server
     **
     ** @param lock True the server no longer accept new connection,
     ** false every connection will be accepted as long as maxconnection is not reached.
     */
    bool getLock(void) const noexcept;

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
     ** @brief Receive bytes from the server and parse them into package for each connection.
     ** It also add new connection and remove disconnected ones.
     ** Should be called at each tick or as soon as you treated all previous packages.
     */
    void receiveNewData(void) noexcept;

    std::deque<cpn::connectionID_t> getNewlyConnectedClients(void) noexcept;

    std::deque<cpn::connectionID_t> getDisconnectedClients(void) noexcept;

    /**
     ** @brief Let you access to the packages of this connection.
     **
     ** @return std::deque<Package>&
     */
    std::deque<Package> &accessPackages(cpn::connectionID_t cid);

    /**
     ** @brief Create a package class that you should fill with data before sending it.
     ** If the package type does not exist, it will throw an exception.
     **
     ** @param commandType
     ** @return Package
     */
    Package createPackage(CommandType_t commandType);

    std::deque<cpn::connectionID_t> getConnectionList(void) noexcept;

    /**
     ** @brief Send a package to the target cid.
     **
     ** @param protocol
     ** @param pkg
     */
    ssize_t sendPackage(int protocol, cpn::connectionID_t cid, const Package &pkg) const;
    /**
     ** @brief Send a package to multiple target cid.
     **
     ** @param protocol
     ** @param pkg
     */
    std::deque<ssize_t> sendPackage(int protocol, const std::deque<cpn::connectionID_t> &cid, const Package &pkg) const;
    /**
     ** @brief Send a list of packages to the target cid.
     **
     ** @param protocol
     ** @param pkgs
     */
    ssize_t sendPackages(int protocol, cpn::connectionID_t cid, const std::deque<Package> &pkgs) const;
    /**
     ** @brief Send a list of packages to the target cid.
     **
     ** @param protocol
     ** @param pkgs
     */
    std::deque<ssize_t> sendPackages(int protocol, const std::deque<cpn::connectionID_t> &cid, const std::deque<Package> &pkgs) const;
    /**
     ** @brief Send a package without body to the target cid.
     ** It can send any package even the ones with a body
     ** without one, so be careful of what you send since it can cause instabilities.
     **
     ** @param protocol
     ** @param commandType
     */
    ssize_t sendBodylessPackage(int protocol, cpn::connectionID_t cid, CommandType_t commandType);
    /**
     ** @brief Send a package without body to the target cids.
     ** It can send any package even the ones with a body
     ** without one, so be careful of what you send since it can cause instabilities.
     **
     ** @param protocol
     ** @param commandType
     */
    std::deque<ssize_t> sendBodylessPackage(int protocol, const std::deque<cpn::connectionID_t> &cid, CommandType_t commandType);
    /**
     ** @brief Send a response package to the target cid.
     **
     ** @param protocol
     ** @param commandType
     ** @param status
     */
    ssize_t sendResponse(int protocol, cpn::connectionID_t cid, CommandType_t commandType, ResponseStatus_t status);
    /**
     ** @brief Send a response package to the target cid.
     **
     ** @param protocol
     ** @param commandType
     ** @param status
     */
    std::deque<ssize_t> sendResponse(int protocol, const std::deque<cpn::connectionID_t> &cid, CommandType_t commandType, ResponseStatus_t status);

    /**
     ** @brief Write the package in a temporary buffer, to be sent later.
     **
     ** @param protocol
     ** @param pkg
     */
    void sendPackageToQueue(int protocol, cpn::connectionID_t cid, const Package &pkg) noexcept;
    /**
     ** @brief Write the package in a temporary buffer, to be sent later.
     **
     ** @param protocol
     ** @param pkg
     */
    std::deque<ssize_t> sendPackageToQueue(int protocol, const std::deque<cpn::connectionID_t> &cid, const Package &pkg) noexcept;
    /**
     ** @brief Write the packages in a temporary buffer, to be sent later.
     **
     ** @param protocol
     ** @param pkgs
     */
    void sendPackagesToQueue(int protocol, cpn::connectionID_t cid, const std::deque<Package> &pkgs) noexcept;
    /**
     ** @brief Write the packages in a temporary buffer, to be sent later.
     **
     ** @param protocol
     ** @param pkgs
     */
    std::deque<ssize_t> sendPackagesToQueue(int protocol, const std::deque<cpn::connectionID_t> &cid, const std::deque<Package> &pkgs) noexcept;
    /**
     ** @brief Write a package without body in a temporary buffer, to be sent later.
     **
     ** @param protocol
     ** @param commandType
     */
    void sendBodylessPackageToQueue(int protocol, cpn::connectionID_t cid, CommandType_t commandType) noexcept;
    /**
     ** @brief Write a package without body in a temporary buffer, to be sent later.
     **
     ** @param protocol
     ** @param commandType
     */
    std::deque<ssize_t> sendBodylessPackageToQueue(int protocol, const std::deque<cpn::connectionID_t> &cid, CommandType_t commandType) noexcept;
    /**
     ** @brief Write a response package in a temporary buffer, to be sent later.
     **
     ** @param protocol
     ** @param commandType
     ** @param status
     */
    void sendResponseToQueue(int protocol, cpn::connectionID_t cid, CommandType_t commandType, ResponseStatus_t status) noexcept;
    /**
     ** @brief Write a response package in a temporary buffer, to be sent later.
     **
     ** @param protocol
     ** @param commandType
     ** @param status
     */
    std::deque<ssize_t> sendResponseToQueue(int protocol, const std::deque<cpn::connectionID_t> &cid, CommandType_t commandType, ResponseStatus_t status) noexcept;

    /**
     ** @brief Send all packages in the temporary buffer.
     **
     ** @param protocol
     */
    void sendQueuedPackages(int protocol);

    cpn::Server &server(void) noexcept;

    void clearAllPackages(void) noexcept;

protected:
    cpn::Server _server;

    PackageFactory _pkgFactory;
    std::unordered_map<uint64_t, ClientPackages_t> _clients;
    std::deque<cpn::connectionID_t> _disconnectedClients;
    std::deque<cpn::connectionID_t> _newlyConnectedClients;

    Header _bodylessHeader;
    Package _response;
private:
};
