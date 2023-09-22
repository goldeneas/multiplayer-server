#pragma once

#include <memory>
#include <SFML/Network.hpp>

class Client {
public:
	using ID = sf::Uint32;
	using Pointer = std::unique_ptr<Client>;
private:
	static inline ID nextFreeId = 0;

	Client::ID id;
    unsigned short port;
	sf::IpAddress address;
public:
	Client(sf::IpAddress address, unsigned short port)
        : address(address), port(port), id(nextFreeId) { ++nextFreeId; };

	[[nodiscard]] Client::ID getId() const { return id; }
    [[nodiscard]] unsigned short getPort() const { return port; }
    [[nodiscard]] sf::IpAddress getAddress() const { return address; }
};