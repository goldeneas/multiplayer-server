#include "ClientHeartbeat.hpp"
#include <spdlog/spdlog.h>

void ClientHeartbeat::handle(ClientHandshakeAccepted e) {
    heartbeats.emplace(e.assignedId, 0);
}

void ClientHeartbeat::handle(IncomingClientHeartbeat e) {
    heartbeats[e.clientId] = 0;
}

void ClientHeartbeat::handle(PlayerLeave e) {
	Client::ID id = e.clientId;

	std::erase_if(heartbeats, [id](auto& heartbeat) {
		auto const& [currentId, currentTime] = heartbeat;
		return id == currentId;
    });
}

void ClientHeartbeat::handle(Tick e) {
	float elapsed = e.dt;

	for(auto it = heartbeats.begin(); it != heartbeats.end();) {
		auto& time = it->second;
		const auto id = it->first;

		time += elapsed;

		if(time >= MAX_NO_REPLY_TIME) {
			it = heartbeats.erase(it);
			server.kickClient(id);

			spdlog::info("A client has timed out. [ID: {}]", id);
		} else {
			++it;
		}
	}
}