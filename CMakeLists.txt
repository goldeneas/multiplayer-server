cmake_minimum_required(VERSION 3.26)
project(multiplayer_server)

set(CMAKE_CXX_STANDARD 20)

set(IMGUI_DIR /opt/nicola/imgui-1.89.9)
set(IMGUI_ADDITIONAL_DIR /opt/nicola/imgui-additional)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

find_library(OPENGL_LIBRARY OpenGl)
find_library(FMT_LIBRARY fmt)
find_library(SPDLOG_LIBRARY spdlog)
find_library(SFML_LIBRARY sfml)

include_directories(
        /opt/homebrew/Cellar/sfml/2.6.0/include
        /opt/homebrew/Cellar/spdlog/1.12.0/include
        /opt/homebrew/Cellar/fmt/10.1.1/include
)

link_directories(
        /opt/homebrew/Cellar/sfml/2.6.0/lib
        /opt/homebrew/Cellar/spdlog/1.12.0/lib
        /opt/homebrew/Cellar/fmt/10.1.1/lib
)

add_executable(multiplayer_server
        Client.hpp
        ClientStack.cpp
        ClientStack.hpp
        EventBus.cpp
        EventBus.hpp
        Events.hpp
        HeartbeatListener.cpp
        HeartbeatListener.hpp
        IEventListener.hpp
        main.cpp
        NetworkServer.cpp
        NetworkServer.hpp
        PacketProcessor.cpp
        PacketProcessor.hpp
        PacketType.cpp
        PacketType.hpp
        PacketWrapper.hpp
)

target_link_libraries(multiplayer_server
        sfml-network sfml-system
        ${SPDLOG_LIBRARY}
        ${FMT_LIBRARY}
        ${OPENGL_LIBRARY}
)