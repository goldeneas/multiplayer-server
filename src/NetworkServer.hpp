#pragma once
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/SocketSelector.hpp>
#include "EventBus.hpp"
#include "PacketType.hpp"
#include "ClientStack.hpp"
#include "IPacketWrapper.hpp"
#include "PacketProcessor.hpp"

class NetworkServer {
private:
	const int localPort;
	ClientStack& clientStack;

	sf::UdpSocket socket;
public:
	NetworkServer(ClientStack& clientStack, int localPort);

	void poll();
	void send(IPacketWrapper& p, Client::ID id);
    void send(IPacketWrapper& p, sf::IpAddress sourceAddress, unsigned short sourcePort);
	void broadcast(IPacketWrapper& p);
	void broadcastExcept(IPacketWrapper& p, Client::ID excludedId);
    void kickClient(Client::ID id);
private:
	void processHandshake(sf::IpAddress sourceAddress, unsigned short sourcePort);
};