// /*
// ** EPITECH PROJECT, 2023
// ** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
// ** File description:
// ** RunLobby
// */

// #include "RTypeServer.hpp"

// void RTypeServer::runLobby(void)
// {
//     this->checkAuthentication();
//     this->LobbyHandlePackage();
// }

// void RTypeServer::LobbyHandleResponse(ClientInfo_t &cli, const Package &pkg)
// {
//     const Body<Response_t> &body = pkg.getBody<const Body<Response_t>>();
//     CommandType_t cmdType = CMD_CAST(body.get()._commandType);
//     ResponseStatus_t responseStatus = RESP_CAST(body.get()._responseStatus);

//     if (cmdType == START_GAME_LOADING && responseStatus == RESP_OK) {
//         cli.readyForLoading = true;
//     }
// }

// void RTypeServer::LobbyHandlePackage(void)
// {
//     Package pkg;
//     std::deque<Package> pkgs;
//     std::unordered_map<uint64_t, ClientInfo_t>::iterator it;

//     if (this->_waitToSwitchToLoading && !this->_authClients.empty())
//         this->_state = STATUS_LOAD;
//     for (it = this->_authClients.begin(); it != this->_authClients.end(); ++it) {
//         pkgs = this->filterPackages(it->second.cid, it->second.pkgs, {
//             RESPONSE,
//             START_GAME_LOADING
//         });
//         while (!pkgs.empty()) {
//             pkg = pkgs.front();
//             switch (pkg.getHeader().get()._commandType) {
//                 case RESPONSE:
//                     this->LobbyHandleResponse(it->second, pkg);
//                     break;
//                 case START_GAME_LOADING:
//                     this->_waitToSwitchToLoading = true;
//                     break;
//             }
//             pkgs.pop_front();
//         }
//         if (this->_waitToSwitchToLoading && !it->second.readyForLoading) {
//             this->_sender.sendBodylessPackage(it->second.cid, this->_server, START_GAME_LOADING);
//             this->_state = STATUS_LOBBY;
//         }
//     }
// }
