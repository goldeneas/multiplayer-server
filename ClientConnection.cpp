#include "ClientConnection.hpp"
#include <spdlog/spdlog.h>

ClientConnection::ClientConnection()
	: id(connectionCount), socket() {

	++connectionCount;
}