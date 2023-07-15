#pragma once
#include <vector>
#include <memory>
#include "ClientConnection.hpp"

class ConnectionStack {
private:
	const size_t maxConnections;
	std::vector<ClientConnection::Pointer> connections;
public:
	ConnectionStack(const size_t maxSize);

	size_t size() { return connections.size(); }
	size_t maxSize() { return maxConnections; }

	void remove(ClientConnection::ID id);
	void add(ClientConnection::Pointer connection);

	auto& getConnections() { return connections; }
	ClientConnection& getConnection(ClientConnection::ID id);
};