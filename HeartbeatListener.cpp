#include "HeartbeatListener.hpp"
#include <spdlog/spdlog.h>

void HeartbeatListener::handle(ClientAccepted e) {
	beats.emplace(e.assignedId, 0);
}

void HeartbeatListener::handle(IncomingClientHeartbeat e) {
	beats[e.clientId] = 0;
}

void HeartbeatListener::handle(ClientLeft e) {
	Client::ID id = e.clientId;

	std::erase_if(beats, [id](auto& heartbeat) {
		auto const& [currentId, currentTime] = heartbeat;
		return id == currentId;
    });
}

void HeartbeatListener::handle(Tick e) {
	float elapsed = e.dt;

	for(auto it = beats.begin(); it != beats.end();) {
		auto& time			= it->second;
		const auto id		= it->first;

		time += elapsed;

		if(time >= MAX_NO_REPLY_TIME) {
			spdlog::info("A client [ID: {}] has timed out", id);

			it = beats.erase(it);
			server.disconnectClient(id);
		} else {
			++it;
		}
	}
}