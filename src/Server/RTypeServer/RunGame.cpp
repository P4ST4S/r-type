// /*
// ** EPITECH PROJECT, 2023
// ** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
// ** File description:
// ** RunGame
// */

// #include "RTypeServer.hpp"

// void RTypeServer::runGame(void)
// {
//     this->GameHandlePackage();
//     this->GameHandlePlayerMovement();
//     this->GameHandlePlayerShoot();

//     // Game logic

//     this->SendDestroyedEntities();
//     // this->SendNewEntities();
//     this->SendMovedEntities();

//     this->_registry.runSystems();
//     this->trashUnusedPackages();
// }

// void RTypeServer::GameHandlePackage(void)
// {
//     Package pkg;
//     std::deque<Package> pkgs;
//     std::unordered_map<uint64_t, ClientInfo_t>::iterator it;

//     for (it = this->_authClients.begin(); it != this->_authClients.end(); ++it) {
//         pkgs = PackageParser::extractPackages(it->second.pkgs, {
//             NEW_ENTITIES,
//             GAME_START,
//         });
//         while (!pkgs.empty()) {
//             pkg = pkgs.front();
//             switch (pkg.getHeader().get()._commandType) {
//                 case NEW_ENTITIES:
//                     break;
//                 case GAME_START:
//                     this->_sender.sendBodylessPackage(it->second.cid, this->_server, GAME_START);
//                     break;
//             }
//             pkgs.pop_front();
//         }
//     }
// }

// void RTypeServer::GameHandlePlayerMovement(void)
// {
//     std::deque<Package> pkgs;
//     std::unordered_map<uint64_t, ClientInfo_t>::iterator it;
//     SparseArray<PlayerEventsComponent> &eventArray = this->_registry.getComponents<PlayerEventsComponent>();


//     for (it = this->_authClients.begin(); it != this->_authClients.end(); ++it) {
//         pkgs = PackageParser::extractPackages(it->second.pkgs, {
//             CLIENT_MOVE_EVENT
//         });
//         if (!pkgs.empty() && eventArray[it->second.entityId].has_value()) {
//             const Body<ClientMoveEvent_t> &body = pkgs.back().getBody<const Body<ClientMoveEvent_t>>();
//             eventArray[it->second.entityId].value().updateEvent(body.get());
//             this->_sender.sendResponse(it->second.cid, this->_server, RESP_OK, CLIENT_MOVE_EVENT);
//         }
//     }
// }

// void RTypeServer::GameHandlePlayerShoot(void)
// {
//     std::deque<Package> pkgs;
//     std::unordered_map<uint64_t, ClientInfo_t>::iterator it;
//     SparseArray<PlayerEventsComponent> &eventArray = this->_registry.getComponents<PlayerEventsComponent>();


//     for (it = this->_authClients.begin(); it != this->_authClients.end(); ++it) {
//         pkgs = PackageParser::extractPackages(it->second.pkgs, {
//             CLIENT_SHOOT_EVENT
//         });
//         if (!pkgs.empty() && eventArray[it->second.entityId].has_value()) {
//             const Body<ClientShootEvent_t> &body = pkgs.back().getBody<const Body<ClientShootEvent_t>>();
//             eventArray[it->second.entityId].value().updateEvent(body.get());
//             this->_sender.sendResponse(it->second.cid, this->_server, RESP_OK, CLIENT_SHOOT_EVENT);
//         }
//     }
// }

// void RTypeServer::SendDestroyedEntities(void)
// {
//     DynamicBody<DestroyEntities_t> &destroyEntitiesBody = this->_destroyEntitiesPkg.getBody<DynamicBody<DestroyEntities_t>>();
//     Zipper<SparseArray<StatusComponent>> zipper(this->_registry.getComponents<StatusComponent>());
//     DestroyEntities_t tmp;

//     destroyEntitiesBody.clear();

//     for (auto it = zipper.begin(); it != zipper.end(); ++it) {
//         StatusComponent &status = std::get<StatusComponent&>(*it);
//         if (status.getStatus() == EntityStatus::DYING) {
//             tmp._id = it.getIndex();
//             destroyEntitiesBody.add(tmp);
//         }
//     }

//     this->_destroyEntitiesPkg.updateBodySize();
//     if (destroyEntitiesBody.size() > 0)
//         this->sendDataToAll(TARGET_AUTH, this->_destroyEntitiesPkg);
// }

// void RTypeServer::SendNewEntities(void)
// {
//     DynamicBody<NewEntities_t> &newEntitiesBody = this->_newEntitiesPkg.getBody<DynamicBody<NewEntities_t>>();
//     Zipper<SparseArray<StatusComponent>> zipper(this->_registry.getComponents<StatusComponent>());

//     newEntitiesBody.clear();

//     for (auto it = zipper.begin(); it != zipper.end(); ++it) {
//         StatusComponent &status = std::get<StatusComponent&>(*it);
//         if (status.getStatus() == EntityStatus::SPAWNING) {
//             newEntitiesBody.add({static_cast<uint16_t>(it.getIndex()), 0});
//         }
//     }

//     this->_newEntitiesPkg.updateBodySize();
//     if (newEntitiesBody.size() > 0) {
//         std::cout << newEntitiesBody.size() << std::endl;
//         this->sendDataToAll(TARGET_AUTH, this->_newEntitiesPkg);
//     }
// }

// void RTypeServer::SendMovedEntities(void)
// {
//     MoveEntitiesDyn_t tmp;
//     DynamicBody<MoveEntitiesDyn_t> &moveEntitiesBody = this->_moveEntitiesPkg.getBody<DynamicBody<MoveEntitiesDyn_t>>();
//     SparseArray<InterpolationComponent> &interpolationArray = this->_registry.getComponents<InterpolationComponent>();

//     moveEntitiesBody.clear();

//     for (size_t i = 0; i < interpolationArray.size(); i++) {
//         if (interpolationArray[i].has_value()) {
//             interpolationArray[i].value().setMoveEntityBody(tmp);
//             tmp._id = i;
//             moveEntitiesBody.add(tmp);
//         }
//     }

//     this->_moveEntitiesPkg.updateBodySize();
//     if (moveEntitiesBody.size() > 0) {
//         std::cout << moveEntitiesBody.size() << std::endl;
//         std::cout << this->_moveEntitiesPkg.getHeader().get()._commandType << std::endl;
//         std::cout << this->_moveEntitiesPkg.getHeader().get()._bodySize << std::endl;
//         this->sendDataToAll(TARGET_AUTH, this->_moveEntitiesPkg);
//     }
// }
