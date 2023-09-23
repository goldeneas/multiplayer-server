#pragma once
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/SocketSelector.hpp>
#include "EventBus.hpp"
#include "PacketType.hpp"
#include "ClientStack.hpp"
#include "PacketWrapper.hpp"
#include "PacketProcessor.hpp"

class NetworkServer {
private:
	const int localPort;
	ClientStack& clientStack;

	sf::UdpSocket socket;
public:
	NetworkServer(ClientStack& clientStack, int localPort);

	void poll();

	void send(PacketWrapper& p, Client::ID id);
	void broadcast(PacketWrapper& p);
	void broadcastExcept(PacketWrapper& p, Client::ID excludedId);
    void kickClient(Client::ID id);
private:
	void processIncomingPackets();
	void processHandshake(sf::IpAddress& sourceAddress, unsigned short sourcePort);
};