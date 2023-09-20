#pragma once
#include "Client.hpp"

struct S2STick {
	float dt;
    explicit S2STick(float dt) : dt(dt) {}
};

struct C2SConnection {
    Client::ID clientId;
    explicit C2SConnection(Client::ID clientId) : clientId(clientId) {}
};

struct C2SDisconnection {
    Client::ID clientId;
    explicit C2SDisconnection(Client::ID clientId) : clientId(clientId) {}
};

struct C2SHeartbeatPacket {
    Client::ID clientId;
    explicit C2SHeartbeatPacket(Client::ID clientId) : clientId(clientId) {}
};

struct S2CPacketPreprocess {
	Client::ID clientId;
	sf::Packet& packet;

    S2CPacketPreprocess(Client::ID clientId, sf::Packet& packet)
    : clientId(clientId), packet(packet) {}
};

struct C2SPacketPreprocess {
	Client::ID clientId;
	sf::Packet& packet;

    C2SPacketPreprocess(Client::ID clientId, sf::Packet& packet)
        : clientId(clientId), packet(packet) {}
};