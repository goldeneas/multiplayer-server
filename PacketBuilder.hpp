#pragma once
#include <SFML/Network.hpp>
#include "PacketType.hpp"

namespace PacketBuilder {

    // https://stackoverflow.com/questions/7230621/how-can-i-iterate-over-a-packed-variadic-template-argument-list
    template<typename... Args>
    sf::Packet build(PacketType type, Args&&... args) {
        int i = 0;
        sf::Packet p;

        p << type;

        ([&]
            {
                ++i;
                p << args;
            } (), ...);

        return p;
    }
}