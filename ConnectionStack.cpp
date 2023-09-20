#include "ConnectionStack.hpp"
#include <spdlog/spdlog.h>

ConnectionStack::ConnectionStack(const size_t maxConnections) : maxConnections(maxConnections) {
	connections.reserve(maxConnections);
}

void ConnectionStack::add(Client::Pointer connection) {
	spdlog::debug("Socket with ID: {} has been added to the socket stack", connection->getId());
	connections.push_back(std::move(connection));
}

void ConnectionStack::remove(Client::ID id) {
	spdlog::debug("Socket with ID: {} has been removed from the socket stack", id);
	std::erase_if(connections, [id](auto& s) { return id == s->getId(); });
}

Client& ConnectionStack::getConnection(Client::ID id) {
	for(auto& connection : connections)
		if(connection->getId() == id)
			return *connection;

	spdlog::error("Socket with ID: {} was not found", id);
}