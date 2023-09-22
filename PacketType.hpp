#pragma once
#include <SFML/Network/Packet.hpp>

enum class PacketType {
    UNDEFINED, C2S_HANDSHAKE, S2C_HANDSHAKE, S2C_PLAYERJOIN, S2C_PLAYERLEAVE
};

sf::Packet& operator>>(sf::Packet& packet, PacketType&& packetType);
sf::Packet& operator<<(sf::Packet& packet, PacketType&& packetType);
sf::Packet& operator>>(sf::Packet& packet, PacketType& packetType);
sf::Packet& operator<<(sf::Packet& packet, PacketType& packetType);