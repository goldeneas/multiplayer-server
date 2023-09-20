#pragma once

#include <memory>
#include <SFML/Network.hpp>

class Client {
public:
	using ID = sf::Uint32;
	using Pointer = std::unique_ptr<Client>;
private:
	static inline ID nextFreeId = 0;

	sf::TcpSocket socket;
	Client::ID id;
public:
	Client();

	auto& getSocket() { return socket; }
	Client::ID getId() const { return id; }
};