#pragma once
#include "ClientConnection.hpp"

struct S2STick {
	float dt;
};

struct C2SConnection {
	ClientConnection::ID connectionId;
};

struct C2SDisconnection {
	ClientConnection::ID connectionId;
};

struct S2CPacketPreprocess {
	ClientConnection::ID connectionId;
	sf::Packet& packet;
};

struct C2SPacketPreprocess {
	ClientConnection::ID connectionId;
	sf::Packet& packet;
};

struct C2SHeartbeatPacket {
	ClientConnection::ID connectionId;
	sf::Packet& packet;
};