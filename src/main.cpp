#include "net/address.hpp"
#include "net/socket.hpp"

#include <spdlog/spdlog.h>

#include <list>
#include <stdexcept>

#include <stdlib.h>
#include <string.h>

//----------------------------------------------------------------------------
int main(int argc, char* argv[])
try {
	std::list<net::TCPServer<net::ip_address<net::ip4_stream>>> servers;

	for (int i = 1; i < argc; ++i) {
		if (char* p = strchr(argv[i], ':')) {
			*p = 0;
			const char* host{ argv[i] };
			const uint16_t port{ static_cast<uint16_t>(atoi(p + 1)) };
			servers.emplace_back(net::ip_address<net::ip4_stream>(host, port));
		}
		else
			throw std::runtime_error{ fmt::format("bad address: \"{}\"", argv[i]) };
	}
}
catch (const std::exception& e) {
	spdlog::error("fatal: file{} line={} text={}", __FILE__, __LINE__, e.what());
}
