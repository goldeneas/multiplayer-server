#include "ClientHeartbeat.hpp"
#include <spdlog/spdlog.h>

void ClientHeartbeat::handle(ClientHandshakeAccepted e) {
    heartbeats.emplace(e.assignedId, std::make_pair(0, 0));
}

void ClientHeartbeat::handle(IncomingClientHeartbeat e) {
    auto& pair = heartbeats[e.clientId];

    pair.first = 0;
    pair.second = 0;
}

void ClientHeartbeat::handle(PlayerLeave e) {
	Client::ID id = e.clientId;

	std::erase_if(heartbeats, [id](auto& heartbeat) {
		auto const& [currentId, pair] = heartbeat;
		return id == currentId;
    });
}

void ClientHeartbeat::handle(Tick e) {
	float elapsed = e.dt;

	for(auto it = heartbeats.begin(); it != heartbeats.end();) {
		const Client::ID id = it->first;
        auto& pair = it->second;

        int& attempts = pair.first;
        float& timeSinceLastHeartbeat = pair.second;

        timeSinceLastHeartbeat += elapsed;

        if(timeSinceLastHeartbeat >= HEARTBEAT_INTERVAL) {
            S2CHeartbeat hb;
            server.send(hb, id);

            spdlog::debug("Sent heartbeat request to client. [ID: {}]", id);

            attempts += 1;
            timeSinceLastHeartbeat = 0;
        }

        if(attempts >= MAX_ATTEMPTS) {
			it = heartbeats.erase(it);
			server.kickClient(id);

			spdlog::info("A client has timed out. [ID: {}]", id);
		} else {
			++it;
		}
	}
}