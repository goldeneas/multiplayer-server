#include "ClientStack.hpp"
#include <spdlog/spdlog.h>

ClientStack::ClientStack(size_t maxSize) : maxClients(maxSize) {
	clients.reserve(maxSize);
}

void ClientStack::add(Client::Pointer client) {
    Client::ID id = client->getId();

    if(id == -1) {
        spdlog::error("Failed adding a client to the client stack! [ID: -1]");
        return;
    }

	spdlog::debug("Client with ID: {} has been added to the client stack", id);
	clients.push_back(std::move(client));
}

void ClientStack::remove(Client::ID id) {
    if(id == -1) {
        spdlog::error("Failed removing a client from the client stack! [ID: -1]");
        return;
    }

	spdlog::debug("Client with ID: {} has been removed from the client stack", id);
	std::erase_if(clients, [id](auto& s) { return id == s->getId(); });
}

bool ClientStack::contains(sf::IpAddress &address, unsigned short port) {
    for(auto& client : clients)
        if (client->getPort() == port && client->getAddress() == address)
            return true;

    return false;
}

Client& ClientStack::getClient(Client::ID id) {
    for(auto& client : clients)
        if(client->getId() == id)
            return *client;

    spdlog::error("Client with ID: {} was not found", id);
}

Client& ClientStack::getClient(sf::IpAddress& address, unsigned short port) {
    for(auto& client : clients)
        if (client->getPort() == port && client->getAddress() == address)
            return *client;

    spdlog::error("Client with address {}:{} was not found", address.toString(), port);
}
