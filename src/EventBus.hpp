#pragma once
#include <list>
#include <utility>
#include "Events.hpp"
#include "IEventListener.hpp"

namespace EventBus {
	extern std::list<IEventListener::Pointer> listeners;

	template<typename Event, typename... Args>
	void emit(Args&&... args) {
		for(auto& listener : listeners)
			listener->handle(Event(std::forward<Args>(args)...));
	};

	template<typename ListenerT, typename... Args>
	void registerListener(Args&&... args) {
		auto listener = std::make_unique<ListenerT>(std::forward<Args>(args)...);
		listeners.push_back(std::move(listener));
	}
}