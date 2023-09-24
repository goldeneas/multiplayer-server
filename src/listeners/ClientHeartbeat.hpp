#pragma once
#include <unordered_map>
#include "NetworkServer.hpp"
#include "IEventListener.hpp"

class ClientHeartbeat : public IEventListener {
private:
    using Attempts = int;
	using TimeSinceLastBeat = float;

    const int MAX_ATTEMPTS = 5;
    const float HEARTBEAT_INTERVAL = 5;
private:
	NetworkServer& server;
	std::unordered_map<Client::ID, std::pair<Attempts , TimeSinceLastBeat>> heartbeats;
public:
	explicit ClientHeartbeat(NetworkServer& server) : server(server) {};
public:
	void handle(Tick e) override;
	void handle(PlayerLeave e) override;
	void handle(ClientHandshakeAccepted e) override;
	void handle(IncomingClientHeartbeat e) override;
};