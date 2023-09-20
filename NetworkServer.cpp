#include "NetworkServer.hpp"
#include <spdlog/spdlog.h>
#include "Events.hpp"
#include "PacketBuilder.hpp"


NetworkServer::NetworkServer(ConnectionStack& connectionStack, const int port)
	: port(port), connectionStack(connectionStack) {
	// setup listener
	if(tcpListener.listen(port) != sf::Socket::Done) {
		spdlog::error("Cannot listen on port {}! Is a listener already attached?", port);
		return;
	}

	selector.add(tcpListener);
	
	spdlog::info("Listener was successfully setup on port {}", port);
}

void NetworkServer::poll() {
	if(selector.wait(sf::seconds(1.0f))) {
		processNewConnections();
		processIncomingPackets();
	}
}

void NetworkServer::processNewConnections() {
	if(!selector.isReady(tcpListener))
		return;

	if(connectionStack.size() >= connectionStack.maxSize()) {
		spdlog::warn("Maximum number of clients has been reached. Closing new connection");
		return;
	}

	Client::Pointer connection = std::make_unique<Client>();
	Client::ID clientId = connection->getId();
	sf::TcpSocket& socket = connection->getSocket();

	if(tcpListener.accept(socket) != sf::Socket::Done) {
		spdlog::error("Something went wrong while trying to accept new connection!");
		return;
	}

	// handle connection
	selector.add(socket);
	connectionStack.add(std::move(connection));
	EventBus::emit<C2SConnection>(clientId);

	spdlog::info("A client [ID: {}] has connected successfully", clientId);

	// broadcast new connection
	spdlog::debug("Broadcasting new connection [ID: {}] to connected clients", clientId);
	broadcastNewConnection(clientId);
}

void NetworkServer::processIncomingPackets() {
	auto& connections = connectionStack.getConnections();

	for(auto it = connections.begin(); it != connections.end();) {
		auto connection = it->get();

		Client::ID clientId = connection->getId();
		sf::TcpSocket& socket = connection->getSocket();

		if(!selector.isReady(socket))
			return;

		sf::Packet p;
		bool hasClientDisconnected = false;

		switch(socket.receive(p)) {
			case sf::Socket::Done: {
				// first, emit general "packet received" event
				EventBus::emit<C2SPacketPreprocess>(clientId, p);
				// then, process the packet and emit correct, specific event
				PacketType t = PacketProcessor::process(p, clientId);
				spdlog::debug("A client [ID: {}] sent packet [TYPE: {}]", clientId, static_cast<int>(t));
				break;
			}

			case sf::Socket::Disconnected:
				hasClientDisconnected = true;
				disconnectClient(clientId, false);
				spdlog::info("A client [ID: {}] has disconnected", clientId);
				break;

			default:
				spdlog::error("A client [ID: {}] tried sending a packet, but an error occurred while receiving!");
				break;
		}

		it = hasClientDisconnected ? connections.erase(it) : ++it;
	}
}

void NetworkServer::broadcastNewConnection(Client::ID id) {
	// broadcast new connection to others
	sf::Packet p = PacketBuilder::build(PacketType::S2C_NEW_CLIENT, id);
	broadcastExcept(id, p);

	// tell new client who's connected
	for(auto& connection : connectionStack.getConnections()) {
		Client::ID otherId = connection->getId();

		if(id == otherId)
			continue;

		sf::Packet p = PacketBuilder::build(PacketType::S2C_NEW_CLIENT, otherId);
		send(id, p);
	}
}

void NetworkServer::send(Client::ID id, sf::Packet& p) {
	auto& connection = connectionStack.getConnection(id);
	auto& socket = connection.getSocket();

	spdlog::debug("Sending packet to ID [{}]", id);

	auto status = socket.send(p);

	if(status != sf::Socket::Done)
		spdlog::error("A client [ID: {}] returned code [{}] after packet was sent!", id, static_cast<int>(status));
}

void NetworkServer::broadcast(sf::Packet& p) {
	for(Client::Pointer& c : connectionStack.getConnections())
		send(c->getId(), p);
}

void NetworkServer::broadcastExcept(Client::ID excludeId, sf::Packet& p) {
	for(Client::Pointer& c : connectionStack.getConnections()) {
		Client::ID currentId = c->getId();

		if(currentId == excludeId)
			continue;

		send(currentId, p);
	}
}

void NetworkServer::disconnectClient(Client::ID id) {
	disconnectClient(id, true);
}

void NetworkServer::disconnectClient(Client::ID id, bool removeFromConnections) {
	auto& connection = connectionStack.getConnection(id);
	auto& socket = connection.getSocket();

	EventBus::emit<C2SDisconnection>(id);
	selector.remove(socket);

	if(removeFromConnections)
		connectionStack.remove(id);
}