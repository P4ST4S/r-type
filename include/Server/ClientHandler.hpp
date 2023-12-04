/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** ClientFilter
*/

#pragma once

#include "RTypeServerStruct.hpp"
#include "Protocol.hpp"

class ClientHandler {
public:
    ClientHandler(ServerPackageHandler &serverPackageHandler);
    ~ClientHandler();

    /**
     ** @brief Add a client to the UNKNOWN client list
     **
     ** @param cid
     */
    void addClient(cpn::connectionID_t cid);
    /**
     ** @brief Remove a client from the client list
     **
     ** @param cid
     */
    void removeClient(cpn::connectionID_t cid);
    std::unordered_map<int, std::unordered_map<uint64_t, ClientInfo_t>> &get(void);
    ClientInfo_t &getClientInfo(cpn::connectionID_t cid);
    int getClientType(cpn::connectionID_t cid);

    std::deque<cpn::connectionID_t> generateCidList(int targetType);

    void moveClientTo(cpn::connectionID_t cid, int targetType);

    Package createPlayerList(void);

    void setMaxAuthClients(size_t maxAuthClients) noexcept;

    size_t getMaxAuthClients(void) const noexcept;

private:
    size_t _maxAuthClients = 3;
    ServerPackageHandler &_pkgHandler;
    std::unordered_map<int, std::unordered_map<uint64_t, ClientInfo_t>> _clients;
};
