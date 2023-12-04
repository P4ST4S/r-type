/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** PackageSender
*/

#pragma once

#include "PackageParser.hpp"

class PackageSender {
public:
    PackageSender(BodyFactory &bodyFactory);
    PackageSender(const PackageSender &other);
    PackageSender(PackageSender &&other);
    ~PackageSender();

    PackageSender operator=(const PackageSender& other) {
        if (this != &other)
            this->copy(other);
        return (*this);
    }

    PackageSender& operator=(PackageSender&& other) {
        if (this != &other) {
            this->_packageParser = std::move(other._packageParser);
            this->_buf = std::move(other._buf);
        }
        return (*this);
    }

    void copy(const PackageSender& other);

    /**
     ** @brief Sends a package.
     **
     ** @param cid The client ID to send the package to.
     ** @param server The server that'll handle the sending.
     ** @param pkg The package to send.
     */
    void sendPackage(cpn::connectionID_t cid, cpn::Server &server, Package &pkg);
    /**
     ** @brief Sends a package.
     **
     ** @param client The client that'll handle the sending.
     ** @param pkg The package to send.
     */
    void sendPackage(cpn::Client &client, Package &pkg);

    /**
     ** @brief Send a package without it's body.
     ** You should use this function when you want
     ** to send a package that doesn't have a body.
     ** It can otherwise provoke instability to the receiver.
     **
     ** @param cid The client ID to send the package to.
     ** @param server The server that'll handle the sending.
     ** @param commandType The command type.
     */
    void sendBodylessPackage(cpn::connectionID_t cid, cpn::Server &server, CommandType_u commandType);
    /**
     ** @brief Send a package without it's body.
     ** You should use this function when you want
     ** to send a package that doesn't have a body.
     ** It can otherwise provoke instability to the receiver.
     **
     ** @param client
     ** @param commandType
     */
    void sendBodylessPackage(cpn::Client &client, CommandType_u commandType);

    /**
     ** @brief Send a response package.
     **
     ** @param cid The client ID to send the package to.
     ** @param server The server that'll handle the sending.
     ** @param status The response status.
     ** @param commandType The command type the response answer.
     */
    void sendResponse(cpn::connectionID_t cid, cpn::Server &server, ResponseStatus_t status, CommandType_t commandType);
    /**
     ** @brief Send a response package.
     **
     ** @param client The client that'll handle the sending.
     ** @param status The response status.
     ** @param commandType The command type the response answer.
     */
    void sendResponse(cpn::Client &client, ResponseStatus_t status, CommandType_t commandType);
    /**
     ** @brief Send a response package.
     **
     ** @param cid The client ID to send the package to.
     ** @param server The server that'll handle the sending.
     ** @param response The response structure.
     */
    void sendResponse(cpn::connectionID_t cid, cpn::Server &server, const Response_t &response);
    /**
     ** @brief Send a response package.
     **
     ** @param client The client that'll handle the sending.
     ** @param response The response structure.
     */
    void sendResponse(cpn::Client &client, const Response_t &response);

    /**
     ** @brief Send a package to the queue.
     **
     ** @param pkg
     */
    void sendPackageToQueue(Package &pkg);
    /**
     ** @brief Send a package without it's body to the queue.
     ** You should use this function when you want
     ** to send a package that doesn't have a body.
     ** It can otherwise provoke instability to the receiver.
     **
     ** @param commandType
     */
    void sendBodylessPackageToQueue(CommandType_u commandType);
    /**
     ** @brief Send a response package to the queue.
     **
     ** @param status
     ** @param commandType
     */
    void sendResponseToQueue(ResponseStatus_t status, CommandType_t commandType);
    /**
     ** @brief Send a response package to the queue.
     **
     ** @param response
     */
    void sendResponseToQueue(const Response_t &response);

    /**
     ** @brief Serialize the queue into vector of byte.
     **
     ** @return std::vector<uint8_t>
     */
    std::deque<uint8_t> &serializeQueue(void);
    /**
     ** @brief Clear the queue.
     **
     */
    void clearQueue(void);

    /**
     ** @brief Send the queue data.
     **
     ** @param cid The client ID to send the package to.
     ** @param server The server that'll handle the sending.
     */
    void sendQueue(cpn::connectionID_t cid, cpn::Server &server, bool clearQueue = false);
    /**
     ** @brief Send the queue data.
     **
     ** @param client The client that'll handle the sending.
     */
    void sendQueue(cpn::Client &client, bool clearQueue = false);

protected:
    Header _header; // Bodyless package.
    Package _response; // Response package.

    PackageParser _packageParser;
    BodyFactory &_bodyFactory;
    cpn::Buffer _buf;
private:
};
