#pragma once

#include <memory>
#include <SFML/Network.hpp>

class ClientConnection {
public:
	using ID = sf::Uint32;
	using Pointer = std::unique_ptr<ClientConnection>;
private:
	static inline ID connectionCount = 0;

	sf::TcpSocket socket;
	ClientConnection::ID id;
public:
	ClientConnection();

	auto& getSocket() { return socket; }
	ClientConnection::ID getId() { return id; }
};