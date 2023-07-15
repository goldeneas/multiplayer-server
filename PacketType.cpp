#include "PacketType.hpp"

sf::Packet& operator>>(sf::Packet& packet, PacketType&& packetType) {
	int intType;
	packet >> intType;

	packetType = static_cast<PacketType>(intType);

	return packet;
}

sf::Packet& operator<<(sf::Packet& packet, PacketType&& packetType) {
	int intType = static_cast<int>(packetType);

	packet << intType;

	return packet;
}

sf::Packet& operator>>(sf::Packet& packet, PacketType& packetType) {
	int intType;
	packet >> intType;

	packetType = static_cast<PacketType>(intType);

	return packet;
}


sf::Packet& operator<<(sf::Packet& packet, PacketType& packetType) {
	int intType = static_cast<int>(packetType);

	packet << intType;

	return packet;
}