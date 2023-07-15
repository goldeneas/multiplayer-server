#pragma once

#include <memory>
#include "Events.hpp"

class IEventListener {
public:
	using Pointer = std::unique_ptr<IEventListener>;
public:
	virtual ~IEventListener() {}

	virtual void handle(S2STick e)				{};
	virtual void handle(C2SHeartbeatPacket e)		{};
	virtual void handle(C2SConnection e)		{};
	virtual void handle(C2SDisconnection e)		{};
	virtual void handle(C2SPacketPreprocess e)	{};
	virtual void handle(S2CPacketPreprocess e)	{};
};