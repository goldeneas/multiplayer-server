#pragma once
#include <SFML/Network/Packet.hpp>

enum class PacketType {
	C2S_HEARTBEAT, S2C_NEW_CLIENT
};

sf::Packet& operator>>(sf::Packet& packet, PacketType&& packetType);
sf::Packet& operator<<(sf::Packet& packet, PacketType&& packetType);
sf::Packet& operator>>(sf::Packet& packet, PacketType& packetType);
sf::Packet& operator<<(sf::Packet& packet, PacketType& packetType);