#include "HeartbeatListener.hpp"
#include <spdlog/spdlog.h>

void HeartbeatListener::handle(C2SConnection e) {
	beats.emplace(e.connectionId, 0);
}

void HeartbeatListener::handle(C2SHeartbeatPacket e) {
	beats[e.connectionId] = 0;
}

void HeartbeatListener::handle(C2SDisconnection e) {
	auto& id = e.connectionId;

	std::erase_if(beats, [id](auto& beat) {
		auto const& [currentId, currentTime] = beat;
		return id == currentId; }
	);
}

void HeartbeatListener::handle(S2STick e) {
	auto elapsed = e.dt;

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