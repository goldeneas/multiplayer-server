#pragma once
#include <utility>

#include "Client.hpp"

struct IncomingPacketPreprocess {
    sf::Packet& p;
    Client::ID clientId;

    IncomingPacketPreprocess(sf::Packet& p, Client::ID clientId)
        : p(p), clientId(clientId) {}
};

struct IncomingClientHandshake {
    unsigned short clientPort;
    std::string clientAddress;

    IncomingClientHandshake(std::string clientAddress, unsigned short clientPort)
        : clientPort(clientPort), clientAddress(std::move(clientAddress)) {}
};

struct IncomingClientHeartbeat {
    Client::ID clientId;
    explicit IncomingClientHeartbeat(Client::ID clientId) : clientId(clientId) {}
};

struct ClientAccepted {
    Client::ID assignedId;
    explicit ClientAccepted(Client::ID assignedId) : assignedId(assignedId) {}
};

struct ClientRefused {

};

struct ClientLeft {
    Client::ID clientId;
    explicit ClientLeft(Client::ID clientId) : clientId(clientId) {}
};

struct Tick {
    float dt;
    explicit Tick(float dt) : dt(dt) {}
};