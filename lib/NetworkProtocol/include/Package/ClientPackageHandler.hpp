/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** ClientPackageHandler
*/

#pragma once

#include "CrossPlatformNetwork.hpp"
#include "PackageFactory.hpp"
#include "PackageParser.hpp"
#include "PackageSender.hpp"

class ClientPackageHandler {
public:
    ClientPackageHandler();
    ClientPackageHandler(const std::string &ip, uint16_t port);
    ~ClientPackageHandler();

    /**
     ** @brief Establish connection with server
     **
     ** @param ip Ip of the server
     ** @param port Port of the server
     ** @return true
     ** @return false
     */
    bool connectTo(const std::string &ip, uint16_t port) noexcept;
    /**
     ** @brief Disconnect the client from the server
     **
     ** @return true
     ** @return false
     */
    bool disconnect(void) noexcept;
    /**
     ** @brief Check if the client is connected to the server
     **
     ** @return true if connected, false otherwise
     */
    bool isConnected() const noexcept;

    /**
     ** @brief Receive bytes from the server and parse them into package.
     ** Should be called at each tick or as soon as you treated all previous packages.
     */
    void receiveNewData(void) noexcept;

    /**
     ** @brief Get up to {nbPkgs} newly parsed packages.
     **
     ** @param nbPkgs Number of packages you want to get. If 0, it will return all packages.
     ** @return std::deque<Package>
     */
    std::deque<Package> getPackages(size_t nbPkgs = 0) noexcept;

    /**
     ** @brief Let you access to the packages of this connection.
     **
     ** @return std::deque<Package>&
     */
    std::deque<Package> &accessPackages(void) noexcept;

    /**
     ** @brief Create a package class that you should fill with data before sending it.
     ** If the package type does not exist, it will throw an exception.
     **
     ** @param commandType
     ** @return Package
     */
    Package createPackage(CommandType_t commandType);

    /**
     ** @brief Send a package to the server.
     **
     ** @param protocol
     ** @param pkg
     */
    ssize_t sendPackage(int protocol, const Package &pkg) const;
    /**
     ** @brief Send a list of packages to the server.
     **
     ** @param protocol
     ** @param pkgs
     */
    ssize_t sendPackages(int protocol, const std::deque<Package> &pkgs) const;
    /**
     ** @brief Send a package without body to the server.
     ** It can send any package even the ones with a body
     ** without one, so be careful of what you send since it can cause instabilities.
     **
     ** @param protocol
     ** @param commandType
     */
    ssize_t sendBodylessPackage(int protocol, CommandType_t commandType);
    /**
     ** @brief Send a response package to the server.
     **
     ** @param protocol
     ** @param commandType
     ** @param status
     */
    ssize_t sendResponse(int protocol, CommandType_t commandType, ResponseStatus_t status);

    /**
     ** @brief Write the package in a temporary buffer, to be sent later.
     **
     ** @param protocol
     ** @param pkg
     */
    void sendPackageToQueue(int protocol, const Package &pkg) noexcept;
    /**
     ** @brief Write the packages in a temporary buffer, to be sent later.
     **
     ** @param protocol
     ** @param pkgs
     */
    void sendPackagesToQueue(int protocol, const std::deque<Package> &pkgs) noexcept;
    /**
     ** @brief Write a package without body in a temporary buffer, to be sent later.
     **
     ** @param protocol
     ** @param commandType
     */
    void sendBodylessPackageToQueue(int protocol, CommandType_t commandType) noexcept;
    /**
     ** @brief Write a response package in a temporary buffer, to be sent later.
     **
     ** @param protocol
     ** @param commandType
     ** @param status
     */
    void sendResponseToQueue(int protocol, CommandType_t commandType, ResponseStatus_t status) noexcept;

    /**
     ** @brief Send all packages in the temporary buffer.
     **
     ** @param protocol
     */
    void sendQueuedPackages(int protocol);

protected:
private:
    void writeInQueue(int protocol, const std::deque<uint8_t> &data) noexcept;

    bool _authenticated = false;
    PackageFactory _pkgFactory;
    cpn::Client _client;
    PackageParser _tcpPkgParser;
    PackageParser _udpPkgParser;
    cpn::Buffer _tcpQueue;
    cpn::Buffer _udpQueue;
    Header _bodylessPkg;
    Package _responsePkg;

    std::deque<Package> _pkgs;
private:
};
