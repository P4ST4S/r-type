/*
** EPITECH PROJECT, 2023
** CrossPlatformNetwork
** File description:
** ReceptionFunctions
*/

#pragma once

#include "INetwork.hpp"
#include "Server.hpp"
#include "CpnUtils.hpp"

void runUdpReception(cpn::INetwork *network);

void runTcpReception(cpn::INetwork *network, bool acceptNewConnection);
