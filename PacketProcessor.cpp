#include "PacketProcessor.hpp"
#include <spdlog/spdlog.h>

PacketType PacketProcessor::process(sf::Packet& packet, Client::ID id) {
	PacketType type;
	packet >> type;

	switch(type)
	{
        case PacketType::C2S_HANDSHAKE: {
            unsigned short clientPort;
            std::string clientAddress;

            packet >> clientAddress;
            packet >> clientPort;

            EventBus::emit<IncomingClientHandshake>(clientAddress, clientPort);
        }

		default:
			spdlog::warn("Tried handling unregistered packet type {}.", static_cast<int>(type));
			break;
	}

	return type;
}