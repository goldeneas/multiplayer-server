#include "ClientStack.hpp"
#include <spdlog/spdlog.h>

ClientStack::ClientStack(size_t maxSize) : maxClients(maxSize) {
	clients.reserve(maxSize);
}

void ClientStack::add(Client::Pointer client) {
	spdlog::debug("Client with ID: {} has been added to the socket stack", client->getId());
	clients.push_back(std::move(client));
}

void ClientStack::remove(Client::ID id) {
	spdlog::debug("Client with ID: {} has been removed from the socket stack", id);
	std::erase_if(clients, [id](auto& s) { return id == s->getId(); });
}

Client& ClientStack::getClient(Client::ID id) {
	for(auto& client : clients)
		if(client->getId() == id)
			return *client;

	spdlog::error("Client with ID: {} was not found", id);
}

Client& ClientStack::getClient(sf::IpAddress& address, unsigned short port) {
    for(auto& client : clients)
        if(client->getPort() == port && client->getAddress() == address)
            return *client;

    spdlog::error("Client with address {}:{} was not found", address.toString(), port);
}
