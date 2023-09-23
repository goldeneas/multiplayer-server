#include "NetworkServer.hpp"
#include <spdlog/spdlog.h>
#include "Events.hpp"

NetworkServer::NetworkServer(ClientStack& clientStack, int localPort)
	: localPort(localPort), clientStack(clientStack) {

	if(socket.bind(localPort) != sf::Socket::Done) {
		spdlog::error("Cannot startup a server! Is port {} busy?", localPort);
		return;
	}

    socket.setBlocking(false);
	spdlog::info("Listener was successfully setup on port {}", localPort);
}

void NetworkServer::poll() {
    sf::Packet p;
    sf::IpAddress sourceAddress;
    unsigned short sourcePort;

    sf::Socket::Status status = socket.receive(p, sourceAddress, sourcePort);
    if(status == sf::Socket::Error) {
        spdlog::error("Could not read packet from {}:{}!",
                      sourceAddress.toString(), sourcePort);
        return;
    }

    if(status != sf::Socket::Done)
        return;

    if(!clientStack.contains(sourceAddress, sourcePort)) {
        processHandshake(sourceAddress, sourcePort);
        return;
    }

    Client& client = clientStack.getClient(sourceAddress, sourcePort);
    Client::ID id = client.getId();

    EventBus::emit<IncomingPacketPreprocess>(p, id);
    PacketType t = PacketProcessor::process(p, id);

    spdlog::debug("A client [{}:{}; {}] has sent a packet [TYPE: {}]",
                  sourceAddress.toString(), sourcePort, id, static_cast<int>(t));
}

void NetworkServer::processHandshake(sf::IpAddress& sourceAddress, unsigned short sourcePort) {
    if(clientStack.size() >= clientStack.maxSize()) {
        EventBus::emit<ClientHandshakeRefused>();
        spdlog::info("Rejected handshake from {}:{}. Maximum amount of clients reached");
        return;
    }

    Client::Pointer c = std::make_unique<Client>(sourceAddress, sourcePort);
    Client::ID id = c->getId();

    clientStack.add(std::move(c));
    EventBus::emit<ClientHandshakeAccepted>(id);
    spdlog::info("Accepted handshake coming from {}:{}. [ASSIGNED-ID: {}]",
                 sourceAddress.toString(), sourcePort, id);
}

//void NetworkServer::processNewConnections() {
//	if(!selector.isReady(tcpListener))
//		return;
//
//	if(connectionStack.size() >= connectionStack.maxSize()) {
//		spdlog::warn("Maximum number of clients has been reached. Closing new connection");
//		return;
//	}
//
//	Client::Pointer connection = std::make_unique<Client>();
//	Client::ID clientId = connection->getId();
//	sf::TcpSocket& socket = connection->getSocket();
//
//	if(tcpListener.accept(socket) != sf::Socket::Done) {
//		spdlog::error("Something went wrong while trying to accept new connection!");
//		return;
//	}
//
//	// handle connection
//	selector.add(socket);
//	connectionStack.add(std::move(connection));
//	EventBus::emit<C2SConnection>(clientId);
//
//	spdlog::info("A client [ID: {}] has connected successfully", clientId);
//
//	// broadcast new connection
//	spdlog::debug("Broadcasting new connection [ID: {}] to connected clients", clientId);
//	broadcastNewConnection(clientId);
//}

//void NetworkServer::broadcastNewConnection(Client::ID id) {
//	// broadcast new connection to others
//	sf::Packet p = PacketBuilder::build(PacketType::S2C_NEW_CLIENT, id);
//	broadcastExcept(id, p);
//
//	// tell new client who's connected
//	for(auto& connection : connectionStack.getAll()) {
//		Client::ID otherId = connection->getId();
//
//		if(id == otherId)
//			continue;
//
//		sf::Packet p = PacketBuilder::build(PacketType::S2C_NEW_CLIENT, otherId);
//		send(id, p);
//	}
//}

void NetworkServer::send(PacketWrapper& p, Client::ID id) {
    Client& client = clientStack.getClient(id);

    PacketType t = p.type;
    sf::Packet packet = p.build();
    unsigned short port = client.getPort();
    sf::IpAddress address = client.getAddress();

    sf::Socket::Status status = socket.send(packet, address, port);
    if(status != sf::Socket::Done) {
        spdlog::error("Could not send packet to client at {}:{}. [TYPE: {}]",
                      address.toString(), port, static_cast<int>(t));
    }
}

void NetworkServer::broadcast(PacketWrapper& p) {
	for(Client::Pointer& c : clientStack.getAll())
		send(p, c->getId());
}

void NetworkServer::broadcastExcept(PacketWrapper& p, Client::ID excludedId) {
	for(Client::Pointer& c : clientStack.getAll()) {
        Client::ID currentId = c->getId();

		if(currentId == excludedId)
			continue;

		send(p, currentId);
	}
}

void NetworkServer::kickClient(Client::ID id) {
    S2CPlayerLeave pl;
    pl.id = id;

    broadcast(pl);
    EventBus::emit<PlayerLeave>(id);
    clientStack.remove(id);
}

//void NetworkServer::disconnectClient(Client::ID id) {
//	disconnectClient(id, true);
//}
//
//void NetworkServer::disconnectClient(Client::ID id, bool removeFromConnections) {
//	auto& connection = connectionStack.getClient(id);
//	auto& socket = connection.getSocket();
//
//	EventBus::emit<C2SDisconnection>(id);
//	selector.remove(socket);
//
//	if(removeFromConnections)
//		connectionStack.remove(id);
//}