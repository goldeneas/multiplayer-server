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

    if(clientStack.contains(sourceAddress, sourcePort)) {
        Client& client = clientStack.getClient(sourceAddress, sourcePort);
        Client::ID id = client.getId();

        EventBus::emit<IncomingPacketPreprocess>(p, id);
        PacketType t = PacketProcessor::process(p, id);

        spdlog::debug("A client [{}:{}; ID: {}] has sent a packet [TYPE: {}]",
                      sourceAddress.toString(), sourcePort, id, static_cast<int>(t));
        return;
    }

    if(PacketProcessor::getType(p) == PacketType::C2S_HANDSHAKE)
        processHandshake(sourceAddress, sourcePort);
    else
        spdlog::warn("A client [{}:{}] tried communicating with us, but hasn't sent an handshake yet",
                     sourceAddress.toString(), sourcePort);
}

void NetworkServer::processHandshake(sf::IpAddress sourceAddress, unsigned short sourcePort) {
    if(clientStack.size() >= clientStack.maxSize()) {
        EventBus::emit<ClientHandshakeRefused>();
        spdlog::info("Rejected handshake from {}:{}. Maximum amount of clients reached");

        S2CHandshake hs;
        hs.isHandshakeSuccessful = false;
        send(hs, sourceAddress, sourcePort);

        return;
    }

    Client::Pointer c = std::make_unique<Client>(sourceAddress, sourcePort);
    Client::ID id = c->getId();

    clientStack.add(std::move(c));
    EventBus::emit<ClientHandshakeAccepted>(id);
    spdlog::info("Accepted handshake coming from {}:{}. [ASSIGNED-ID: {}]",
                 sourceAddress.toString(), sourcePort, id);

    S2CHandshake hs;
    hs.assignedId = id;
    hs.isHandshakeSuccessful = true;
    send(hs, sourceAddress, sourcePort);
}

void NetworkServer::send(IPacketWrapper& p, sf::IpAddress sourceAddress, unsigned short sourcePort) {
    PacketType t = p.type;
    sf::Packet packet = p.build();

    sf::Socket::Status status = socket.send(packet, sourceAddress, sourcePort);
    if(status != sf::Socket::Done) {
        spdlog::error("Could not send packet to client at {}:{}. [TYPE: {}]",
                      sourceAddress.toString(), sourcePort, static_cast<int>(t));
    }
}

void NetworkServer::send(IPacketWrapper& p, Client::ID id) {
    Client& client = clientStack.getClient(id);
    send(p, client.getAddress(), client.getPort());
}

void NetworkServer::broadcast(IPacketWrapper& p) {
	for(Client::Pointer& c : clientStack.getAll())
		send(p, c->getId());
}

void NetworkServer::broadcastExcept(IPacketWrapper& p, Client::ID excludedId) {
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