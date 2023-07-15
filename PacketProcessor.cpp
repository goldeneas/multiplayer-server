#include "PacketProcessor.hpp"
#include <spdlog/spdlog.h>

PacketType PacketProcessor::process(sf::Packet& packet, ClientConnection::ID id) {
	PacketType type;
	packet >> type;

	switch(type)
	{
		case PacketType::C2S_HEARTBEAT:
			EventBus::emit<C2SHeartbeatPacket>(id, packet);
			break;

		default:
			spdlog::warn("Tried handling unregistered packet type {}.", static_cast<int>(type));
			break;
	}

	return type;
}