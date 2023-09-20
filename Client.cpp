#include "Client.hpp"
#include <spdlog/spdlog.h>

Client::Client()
	: id(nextFreeId), socket() {

	++nextFreeId;
}