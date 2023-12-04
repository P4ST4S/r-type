/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** HandleDisconnectionPackage
*/

#include "HandleInformationPackageSystem.hpp"

HandleInformationPackageSystem::HandleInformationPackageSystem(ServerPackageHandler &serverPackageHandler, ClientHandler &clientHandler) :
_pkgHandler(serverPackageHandler), _cliHandler(clientHandler)
{
}

HandleInformationPackageSystem::~HandleInformationPackageSystem()
{
}

void HandleInformationPackageSystem::operator()(Registry &registry)
{
    // std::cout << "HANDLE INFORMATION PACKAGE SYSTEM" << std::endl;
    Package pkg;
    std::deque<cpn::connectionID_t> cids = this->_pkgHandler.getConnectionList();
    std::deque<Package> pkgs;
    size_t entityId;

    for (size_t i = 0; i < cids.size(); ++i) {
        pkgs = PackageParser::extractPackages(this->_pkgHandler.accessPackages(cids[i]), {
            RESPONSE
        });
        while (!pkgs.empty()) {
            pkg = pkgs.front();
            pkgs.pop_front();
            Body<Response_t> &body = pkg.getBody<Body<Response_t>>();

            if (body.get()._responseStatus == RESP_INFORMATION) {
                switch (body.get()._commandType) {
                    case PLAYER_LIST:
                        this->_pkgHandler.sendPackage(CPN_TCP, cids[i], this->_cliHandler.createPlayerList());
                        break;
                    default:
                        this->_pkgHandler.sendResponse(CPN_TCP, cids[i], CMD_CAST(body.get()._commandType), RESP_KO);
                        break;
                }
            } else {
                this->_pkgHandler.accessPackages(cids[i]).push_back(pkg);
            }
        }
    }
}
