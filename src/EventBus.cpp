#include "EventBus.hpp"

namespace EventBus {
	std::list<IEventListener::Pointer> listeners;
}