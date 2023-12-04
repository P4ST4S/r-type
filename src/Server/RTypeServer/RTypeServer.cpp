/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** RTypeServer
*/

#include "RTypeServer.hpp"

bool *RTypeServerRunning;

static void stopRTypeServer(int signl_num)
{
    *RTypeServerRunning = false;
    std::cout << "\nManual stop detected. Server about to stop." << std::endl;
}

RTypeServer::RTypeServer() :
_cliHandler(this->_pkgHandler)
{
    Package tmpPkg = this->_pkgHandler.createPackage(RESPONSE);

    tmpPkg.getBody<Body<Response_t>>().get()._responseStatus = RESP_KO;
    tmpPkg.getBody<Body<Response_t>>().get()._commandType = CLIENT_CONNECTION_REQUEST;
    this->_pkgHandler.dataToSendWhenLock(tmpPkg.serialize());
    tmpPkg.getBody<Body<Response_t>>().get()._responseStatus = RESP_SERVER_FULL;
    tmpPkg.getBody<Body<Response_t>>().get()._commandType = CLIENT_CONNECTION_REQUEST;
    this->_pkgHandler.dataToSendWhenMaxConnection(tmpPkg.serialize());
    tmpPkg.getBody<Body<Response_t>>().get()._responseStatus = RESP_SERVER_FULL;
    tmpPkg.getBody<Body<Response_t>>().get()._commandType = CLIENT_CONNECTION_REQUEST;
    this->_pkgHandler.dataToSendWhenRemove(tmpPkg.serialize());
    tmpPkg.getBody<Body<Response_t>>().get()._responseStatus = RESP_UNAUTHORIZED;
    tmpPkg.getBody<Body<Response_t>>().get()._commandType = RESPONSE;
    this->_pkgHandler.dataToSendWhenConnectionNotConnected(tmpPkg.serialize());

    RTypeServerRunning = &this->_running;


    // ECS
    _registry.registerEvent<NewEntityEvent>();
    _registry.registerEvent<KillEntityEvent>();
    _registry.registerEvent<InputEvent>();
    _registry.registerEvent<PartyEndEvent>();

    _registry.registerComponent<PositionComponent>(destroyPosition);
    _registry.registerComponent<VelocityComponent>(destroyVelocity);
    _registry.registerComponent<InterpolationComponent>(destroyInterpolation);
    _registry.registerComponent<PlayerEventsComponent>(destroyPlayerEvents);
    _registry.registerComponent<PlayerComponent>(destroyPlayer);
    _registry.registerComponent<MobComponent>(destroyMob);
    _registry.registerComponent<BossComponent>(destroyBoss);
    _registry.registerComponent<BulletComponent>(destroyBullet);
    _registry.registerComponent<WeaponComponent>(destroyWeapon);
    _registry.registerComponent<LifeHitboxComponent>(destroyLifeHitbox);
    _registry.registerComponent<DamageHitboxComponent>(destroyDamageHitbox);
    _registry.registerComponent<IdentityComponent>(destroyIdentity);
    //_registry.registerComponent<IdentityComponent>(destroyIdentity);

    _registry.addSystem<LifeHitboxComponent, DamageHitboxComponent, IdentityComponent, InterpolationComponent>(HitboxSystem());
    _registry.addSystem<MobComponent, BossComponent>(WaveSystem(this->_state));
    _registry.addSystem<>(ReceiveNewDataSystem(this->_pkgHandler));
    _registry.addSystem<>(HandleConnectionSystem(this->_pkgHandler, this->_cliHandler));
    _registry.addSystem<>(HandleAuthenticationSystem(this->_pkgHandler, this->_cliHandler));
    _registry.addSystem<>(HandleDisconnectionPackageSystem(this->_pkgHandler, this->_cliHandler));
    _registry.addSystem<>(HandleInformationPackageSystem(this->_pkgHandler, this->_cliHandler));
    _registry.addSystem<>(HandleLobbyStatusSystem(this->_pkgHandler, this->_cliHandler, this->_state));
    _registry.addSystem<>(HandleLoadStatusSystem(this->_pkgHandler, this->_cliHandler, this->_state));
    _registry.addSystem<>(HandleGameStatusSystem(this->_pkgHandler, this->_cliHandler, this->_state));
    _registry.addSystem<>(HandleUnloadStatusSystem(this->_pkgHandler, this->_cliHandler, this->_state));

    _registry.addSystem<InterpolationComponent>(InterpolationMoveSystem());

    _registry.addSystem<>(SendDestroyEntitiesPackageSystem(this->_pkgHandler, this->_cliHandler));
    _registry.addSystem<>(SendNewEntitiesPackageSystem(this->_pkgHandler, this->_cliHandler));

    _registry.addSystem<InterpolationComponent, PlayerEventsComponent, WeaponComponent, VelocityComponent>(UpdateEntitiesEventSystem());
    _registry.addSystem<BulletComponent, DamageHitboxComponent, InterpolationComponent, VelocityComponent>(MissileSystem());


    _registry.addSystem<MobComponent, VelocityComponent, PlayerComponent, WeaponComponent, InterpolationComponent>(HandleMobAISystem(this->_pkgHandler, this->_cliHandler));
    _registry.addSystem<MobComponent, BossComponent, VelocityComponent, PlayerComponent, WeaponComponent, InterpolationComponent>(HandleBossAISystem(this->_pkgHandler, this->_cliHandler));
    // JP Add your system here

    _registry.addSystem<InterpolationComponent>(SendMoveEntitiesPackageSystem(this->_pkgHandler, this->_cliHandler, this->_state));

    // _registry.addSystem<>(StartFrameSystem());

}

RTypeServer::~RTypeServer()
{
}

void RTypeServer::logServerInfo(void)
{
    double speed = (1 / this->_clk.getDeltaTime() * 100) / this->_tickRate;
    double load = 100 * (static_cast<double>(this->_clk.getRealNsTickDuration()) / (1e9 / this->_tickRate));

    std::cout << std::setprecision(2) << std::fixed;
    std::cout << "\r\033[0K" <<
    "Status: " << this->_state <<
    " | Total Cli: " << this->_pkgHandler.server().getConnectionCount() <<
    " | Unknown Cli: " << this->_cliHandler.get()[TARGET_CLI_UNKOWN].size() <<
    " | Auth Cli: " << this->_cliHandler.get()[TARGET_CLI_AUTH].size() <<
    " | Spectator Cli: " << this->_cliHandler.get()[TARGET_CLI_SPECTATOR].size() <<
    " | Speed: " << speed << "%" <<
    " | Load: " << load << "%" <<
    std::flush;
}

bool RTypeServer::run(uint16_t port)
{
    if (!this->_pkgHandler.start(port))
        return (false);
    signal(SIGINT, stopRTypeServer);
    this->_pkgHandler.setLock(false);
    this->_running = true;
    while (this->_running) {
        this->_registry.runSystems();
        this->logServerInfo();
        this->_clk.cap(this->_tickRate);
    }
    signal(SIGINT, SIG_DFL);
    this->_pkgHandler.stop();
    return (true);
}

// void RTypeServer::handleInfoResponse(void)
// {
//     Package pkg;
//     std::deque<Package> pkgs;
//     std::unordered_map<uint64_t, ClientInfo_t>::iterator it;

//     for (it = this->_authClients.begin(); it != this->_authClients.end(); ++it) {
//         pkgs = PackageParser::extractPackages(it->second.pkgs, {
//             RESPONSE,
//         });
//         while (!pkgs.empty()) {
//             pkg = pkgs.front();
//             Body<Response_t> body = pkg.getBody<Body<Response_t>>();
//             switch (body.get()._commandType) {
//                 case PLAYER_LIST:
//                     this->_sender.sendPackage(it->second.cid, this->_server, getPlayers());
//                     break;
//             }
//             pkgs.pop_front();
//         }
//     }
// }


// void RTypeServer::handlePingPackages(void)
// {
//     Package pkg;
//     std::deque<Package> pkgs;
//     std::unordered_map<uint64_t, ClientInfo_t>::iterator it;

//     for (it = this->_authClients.begin(); it != this->_authClients.end(); ++it) {
//         pkgs = PackageParser::extractPackages(it->second.pkgs, {
//             PING,
//             CLIENT_CONNECTION_REQUEST,
//         });
//         while (!pkgs.empty()) {
//             pkg = pkgs.front();
//             switch (pkg.getHeader().get()._commandType) {
//                 case PING:
//                     this->_sender.sendResponse(it->second.cid, this->_server, RESP_OK, PING);
//                     break;
//                 case CLIENT_CONNECTION_REQUEST:
//                     this->_sender.sendResponse(it->second.cid, this->_server, RESP_OK, CLIENT_CONNECTION_REQUEST);
//                     break;
//             }
//             pkgs.pop_front();
//         }
//     }
// }

// void RTypeServer::receivePackages(size_t dataSize)
// {
//     std::unordered_map<uint64_t, ClientInfo_t>::iterator it;
//     std::deque<Package> packages;

//     for (it = this->_clients.begin(); it != this->_clients.end(); ++it) {
//         it->second.parser.writeBytesIn(this->_server.receive(CPN_TCP, it->first, dataSize));
//         // it->second.parser.writeBytesIn(this->_server.receive(CPN_UDP, it->first, dataSize));
//         packages = it->second.parser.getPackages();
//         it->second.pkgs.insert(it->second.pkgs.end(), packages.begin(), packages.end());
//     }
//     for (it = this->_authClients.begin(); it != this->_authClients.end(); ++it) {
//         it->second.parser.writeBytesIn(this->_server.receive(CPN_TCP, it->first, dataSize));
//         // it->second.parser.writeBytesIn(this->_server.receive(CPN_UDP, it->first, dataSize));
//         packages = it->second.parser.getPackages();
//         it->second.pkgs.insert(it->second.pkgs.end(), packages.begin(), packages.end());
//     }
// }

// void RTypeServer::checkAuthentication(void)
// {
//     bool itReset = false;
//     std::unordered_map<uint64_t, ClientInfo_t>::iterator it = this->_clients.begin();
//     Package pkg;
//     CommandType_t cmdType;

//     while (it != this->_clients.end()) {
//         itReset = false;
//         while (!it->second.pkgs.empty()) {
//             pkg = it->second.pkgs.front();
//             it->second.pkgs.pop_front();
//             cmdType = pkg.getHeader().get()._commandType;
//             if (cmdType == CLIENT_CONNECTION_REQUEST) {
//                 if (this->_authClients.size() < this->_maxClients) {
//                     this->_authClients[it->first] = std::move(it->second);
//                     this->_sender.sendResponse(it->first, this->_server, RESP_OK, CLIENT_CONNECTION_REQUEST);
//                     this->_clients.erase(it);
//                     it = this->_clients.begin();
//                     this->newPlayer();
//                     this->sendDataToAll(TARGET_AUTH, getPlayers());
//                     itReset = true;
//                     break;
//                 } else {
//                     this->_sender.sendResponse(it->first, this->_server, RESP_SERVER_FULL, CLIENT_CONNECTION_REQUEST);
//                 }
//             } else {
//                 this->_sender.sendResponse(it->first, this->_server, RESP_KO, cmdType);
//             }
//         }
//         if (!itReset)
//             ++it;
//     }
// }
