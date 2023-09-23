#pragma once
#include <spdlog/spdlog.h>
#include <SFML/Network/Packet.hpp>
#include "Events.hpp"
#include "EventBus.hpp"
#include "PacketType.hpp"
#include "Client.hpp"

// candidate to sharing
namespace PacketProcessor {
	PacketType process(sf::Packet& packet, Client::ID id);
};