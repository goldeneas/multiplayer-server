#pragma once
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/SocketSelector.hpp>
#include "EventBus.hpp"
#include "PacketType.hpp"
#include "ConnectionStack.hpp"
#include "PacketProcessor.hpp"

class NetworkServer {
private:
	const int port;
	ConnectionStack& connectionStack;

	sf::TcpListener tcpListener;
	sf::SocketSelector selector;
public:
	NetworkServer(ConnectionStack& connections, const int port);

	void poll();

	void send(Client::ID id, sf::Packet& p);
	void broadcast(sf::Packet& p);
	void broadcastExcept(Client::ID id, sf::Packet& p);
	void disconnectClient(Client::ID id);
private:
	void processNewConnections();
	void processIncomingPackets();
	void broadcastNewConnection(Client::ID id);
	void disconnectClient(Client::ID id, bool removeFromConnections);
};