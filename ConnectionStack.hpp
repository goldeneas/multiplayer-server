#pragma once
#include <vector>
#include <memory>
#include "Client.hpp"

class ConnectionStack {
private:
	const size_t maxConnections;
	std::vector<Client::Pointer> connections;
public:
	ConnectionStack(const size_t maxSize);

	size_t size() { return connections.size(); }
	size_t maxSize() { return maxConnections; }

	void remove(Client::ID id);
	void add(Client::Pointer connection);

	auto& getConnections() { return connections; }
	Client& getConnection(Client::ID id);
};