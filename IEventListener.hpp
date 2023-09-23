#pragma once

#include <memory>
#include "Events.hpp"

class IEventListener {
public:
	using Pointer = std::unique_ptr<IEventListener>;
public:
	virtual ~IEventListener() = default;

    virtual void handle(Tick e)	{};
    virtual void handle(PlayerLeave e) {};
    virtual void handle(ClientHandshakeRefused e) {};
    virtual void handle(ClientHandshakeAccepted e) {};
    virtual void handle(IncomingClientHandshake e)	{};
    virtual void handle(IncomingClientHeartbeat e)	{};
    virtual void handle(IncomingPacketPreprocess e)	{};
};