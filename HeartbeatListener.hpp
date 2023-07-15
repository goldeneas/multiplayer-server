#pragma once
#include <unordered_map>
#include "NetworkServer.hpp"
#include "IEventListener.hpp"

class HeartbeatListener : public IEventListener {
private:
	using TimeSinceLastBeat = float;
	const float MAX_NO_REPLY_TIME = 60;
private:
	NetworkServer& server;
	std::unordered_map<ClientConnection::ID, TimeSinceLastBeat> beats;
public:
	HeartbeatListener(NetworkServer& server) : server(server) {};
public:
	void handle(S2STick e) override;
	void handle(C2SConnection e) override;
	void handle(C2SDisconnection e) override;
	void handle(C2SHeartbeatPacket e) override;
};