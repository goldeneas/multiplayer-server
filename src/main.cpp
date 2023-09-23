#include <spdlog/spdlog.h>
#include "EventBus.hpp"
#include "ClientStack.hpp"
#include "NetworkServer.hpp"
#include "listeners/ClientHeartbeat.hpp"

int main() {
	ClientStack connectionStack(20);
	NetworkServer server(connectionStack, 39964);

	spdlog::set_level(spdlog::level::debug);

	EventBus::registerListener<ClientHeartbeat>(server);

	sf::Clock clock;
	while(true) {
		sf::Time dt = clock.restart();

		server.poll();
		EventBus::emit<Tick>(dt.asSeconds());
	}

	return 0;
}