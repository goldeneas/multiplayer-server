#pragma once
#include <unordered_map>
#include "NetworkServer.hpp"
#include "IEventListener.hpp"

class ClientHeartbeat : public IEventListener {
private:
	using TimeSinceLastBeat = float;
	const float MAX_NO_REPLY_TIME = 60;
private:
	NetworkServer& server;
	std::unordered_map<Client::ID, TimeSinceLastBeat> heartbeats;
public:
	explicit ClientHeartbeat(NetworkServer& server) : server(server) {};
public:
	void handle(Tick e) override;
	void handle(PlayerLeave e) override;
	void handle(ClientHandshakeAccepted e) override;
	void handle(IncomingClientHeartbeat e) override;
};