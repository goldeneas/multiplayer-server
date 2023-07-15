#include <spdlog/spdlog.h>
#include "EventBus.hpp"
#include "HeartbeatListener.hpp"
#include "NetworkServer.hpp"
#include "ConnectionStack.hpp"

int main() {
	ConnectionStack connectionStack(20);
	NetworkServer server(connectionStack, 39964);

	spdlog::set_level(spdlog::level::debug);

	EventBus::registerListener<HeartbeatListener>(server);

	sf::Clock clock;
	while(true) {
		sf::Time dt = clock.restart();

		server.poll();
		EventBus::emit<S2STick>(dt.asSeconds());
	}

	return 0;
}