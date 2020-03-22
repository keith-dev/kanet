#include "net/address.hpp"
#include "net/socket.hpp"

#include <spdlog/spdlog.h>

#include <list>
#include <exception>

#include <stdlib.h>
#include <string.h>

//----------------------------------------------------------------------------
using ip4_address = net::ip_address<net::ip4_stream>;

int main(int argc, char* argv[])
try {
	std::list<net::TCPServer<ip4_address>> tcpservers;
	for (int i = 1; i < argc; ++i) {
		if (char* p = strchr(argv[i], ':')) {
			*p = 0;
			const char* host{ argv[i] };
			const uint16_t port{ static_cast<uint16_t>(atoi(p + 1)) };
			tcpservers.emplace_back( ip4_address{host, port} );
		}
	}
}
catch (const std::exception& e) {
	spdlog::error("fatal: file{} line={} text={}", __FILE__, __LINE__, e.what());
}
