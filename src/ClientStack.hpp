#pragma once
#include <vector>
#include <memory>
#include "Client.hpp"

class ClientStack {
private:
	const size_t maxClients;
	std::vector<Client::Pointer> clients;
public:
	explicit ClientStack(size_t maxSize);

	size_t size() { return clients.size(); }
	size_t maxSize() { return maxClients; }

	void remove(Client::ID id);
	void add(Client::Pointer client);

	auto& getAll() { return clients; }
	Client& getClient(Client::ID id);
    Client& getClient(sf::IpAddress& address, unsigned short port);
    bool contains(sf::IpAddress& address, unsigned short port);
};