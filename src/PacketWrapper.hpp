#pragma once
#include "Client.hpp"
#include "PacketType.hpp"

// TODO: add a way to recognise clients sending packets

struct PacketWrapper {
    sf::Packet rawPacket{};
    PacketType type = PacketType::UNDEFINED;

    virtual sf::Packet build() = 0;
};

struct C2SHandshake : public PacketWrapper {
    PacketType type = PacketType::C2S_HANDSHAKE;

    unsigned short clientPort{};
    std::string clientAddress{};

    sf::Packet build() override {
        rawPacket << type;
        rawPacket << clientAddress;
        rawPacket << clientPort;

        return rawPacket;
    }
};

struct S2CHandshake : public PacketWrapper {
    PacketType type = PacketType::S2C_HANDSHAKE;

    Client::ID assignedId = -1;
    bool isHandshakeSuccessful = false;

    sf::Packet build() override {
        rawPacket << type;
        rawPacket << isHandshakeSuccessful;
        rawPacket << assignedId;

        return rawPacket;
    }
};

struct S2CPlayerJoin : public PacketWrapper {
    PacketType type = PacketType::S2C_PLAYERJOIN;

    Client::ID id = -1;

    sf::Packet build() override {
        rawPacket << type;
        rawPacket << id;

        return rawPacket;
    }
};

struct S2CPlayerLeave : public PacketWrapper {
    PacketType type = PacketType::S2C_PLAYERLEAVE;

    Client::ID id = -1;

    sf::Packet build() override {
        rawPacket << type;
        rawPacket << id;

        return rawPacket;
    }
};