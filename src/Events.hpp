#pragma once
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

struct ClientHandshakeAccepted {
    Client::ID assignedId;
    explicit ClientHandshakeAccepted(Client::ID assignedId) : assignedId(assignedId) {}
};

struct ClientHandshakeRefused {

};

struct PlayerLeave {
    Client::ID clientId;
    explicit PlayerLeave(Client::ID clientId) : clientId(clientId) {}
};

struct Tick {
    float dt;
    explicit Tick(float dt) : dt(dt) {}
};