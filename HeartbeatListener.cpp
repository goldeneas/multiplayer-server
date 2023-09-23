#include "HeartbeatListener.hpp"
#include <spdlog/spdlog.h>

void HeartbeatListener::handle(ClientHandshakeAccepted e) {
    heartbeats.emplace(e.assignedId, 0);
}

void HeartbeatListener::handle(IncomingClientHeartbeat e) {
    heartbeats[e.clientId] = 0;
}

void HeartbeatListener::handle(PlayerLeave e) {
	Client::ID id = e.clientId;

	std::erase_if(heartbeats, [id](auto& heartbeat) {
		auto const& [currentId, currentTime] = heartbeat;
		return id == currentId;
    });
}

void HeartbeatListener::handle(Tick e) {
	float elapsed = e.dt;

	for(auto it = heartbeats.begin(); it != heartbeats.end();) {
		auto& time			= it->second;
		const auto id		= it->first;

		time += elapsed;

		if(time >= MAX_NO_REPLY_TIME) {
			spdlog::info("A client [ID: {}] has timed out", id);

			it = heartbeats.erase(it);
			server.kickClient(id);
		} else {
			++it;
		}
	}
}