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

	void send(ClientConnection::ID id, sf::Packet& p);
	void broadcast(sf::Packet& p);
	void broadcastExcept(ClientConnection::ID id, sf::Packet& p);
	void disconnectClient(ClientConnection::ID id);
private:
	void processNewConnections();
	void processIncomingPackets();
	void broadcastNewConnection(ClientConnection::ID id);
	void disconnectClient(ClientConnection::ID id, bool removeFromConnections);
};