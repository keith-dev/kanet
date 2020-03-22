#pragma once

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <string_view>
#include <string>

#include <stdint.h>
#include <string.h>

//----------------------------------------------------------------------------

namespace net {
	//------------------------------------------------------------------------
	template <typename ADDR>
	struct ip_address : ADDR {
		using inherited = ADDR;

		ip_address(std::string_view addr_str, uint16_t port = 0) : inherited(addr_str, port) {}
		ip_address(const char* addr_str, uint16_t port = 0) : inherited(addr_str, port) {}
		ip_address(uint16_t port = 0) : inherited(port) {}

		void set(std::string_view addr_str) { inherited::set(addr_str); }
		void set(const char* addr_str) { inherited::set(addr_str); }
		void set(std::string_view addr_str, uint16_t port) { inherited::set(addr_str, port); }
		void set(const char* addr_str, uint16_t port) { inherited::set(addr_str, port); }
		void set(uint16_t port) { inherited::set(port); }
		void set(const ADDR& addr) { inherited::set(addr); }

		uint16_t	port() const { return inherited::port(); }
		std::string	address() const { return inherited::address(); }
		int			family() const { return inherited::family(); }

		operator const sockaddr* () const { return reinterpret_cast<const sockaddr*>(this); }
		operator sockaddr* () { return reinterpret_cast<sockaddr*>(this); }
		socklen_t	size() const { return sizeof(*this); }

		constexpr int domain() const { return inherited::domain; }
		constexpr int type() const { return inherited::type; }
		constexpr int protocol() const { return inherited::protocol; }
	};

	//------------------------------------------------------------------------
	struct ip4 : sockaddr_in {
		using address_type = sockaddr_in;

		ip4(std::string_view addr_str, uint16_t port = 0) {
			sin_len = sizeof(*this);
			sin_family = AF_INET;
			sin_addr.s_addr = inet_addr(addr_str.data());
			sin_port = htons(port);
		}
		ip4(const char* addr_str, uint16_t port = 0) {
			sin_len = sizeof(*this);
			sin_family = AF_INET;
			sin_addr.s_addr = inet_addr(addr_str);
			sin_port = htons(port);
		}
		ip4(uint16_t port = 0) {
			sin_len = sizeof(*this);
			sin_family = AF_INET;
			sin_addr.s_addr = INADDR_ANY;
			sin_port = htons(port);
		}
		void set(std::string_view addr_str) {
			sin_family = AF_INET;
			sin_addr.s_addr = inet_addr(addr_str.data());
		}
		void set(const char* addr_str) {
			sin_family = AF_INET;
			sin_addr.s_addr = inet_addr(addr_str);
		}
		void set(std::string_view addr_str, uint16_t port) {
			set(addr_str);
			sin_port = htons(port);
		}
		void set(const char* addr_str, uint16_t port) {
			set(addr_str);
			sin_port = htons(port);
		}
		void set(uint16_t port) {
			sin_port = htons(port);
		}
		void set(const struct sockaddr_in& addr) {
			memcpy(this, &addr, sizeof(addr));
		}
		uint16_t port() const {
			return ntohs(sin_port);
		}
		std::string address() const {
			return inet_ntoa(sin_addr);
		}
		int family() const {
			return sin_family;
		}
	};

	//------------------------------------------------------------------------
	struct ip6 : sockaddr_in6 {
		using address_type = sockaddr_in6;

		ip6(std::string_view addr_str, uint16_t port = 0) {
			sin6_len = sizeof(*this);
			sin6_family = AF_INET6;
			inet_pton(AF_INET6, addr_str.data(), &sin6_addr);
			sin6_port = htons(port);
		}
		ip6(const char* addr_str, uint16_t port = 0) {
			sin6_len = sizeof(*this);
			sin6_family = AF_INET6;
			inet_pton(AF_INET6, addr_str, &sin6_addr);
			sin6_port = htons(port);
		}
		ip6(uint16_t port = 0) {
			sin6_len = sizeof(*this);
			sin6_family = AF_INET6;
			sin6_addr = in6addr_any;
			sin6_port = htons(port);
		}
		void set(std::string_view addr_str) {
			sin6_family = AF_INET6;
			inet_pton(AF_INET6, std::string(addr_str).c_str(), &sin6_addr);
		}
		void set(const char* addr_str) {
			sin6_family = AF_INET6;
			inet_pton(AF_INET6, addr_str, &sin6_addr);
		}
		void set(std::string_view addr_str, uint16_t port) {
			set(addr_str);
			sin6_port = htons(port);
		}
		void set(const char* addr_str, uint16_t port) {
			set(addr_str);
			sin6_port = htons(port);
		}
		void set(uint16_t port) {
			sin6_port = htons(port);
		}
		void set(struct sockaddr_in6 addr) {
			memcpy(this, &addr, sizeof(addr));
		}
		uint16_t port() const {
			return ntohs(sin6_port);
		}
		std::string address() const {
			char buffer[8*4 + 7*2 + 1];
			return inet_ntop(AF_INET6, &sin6_addr, buffer, sizeof(buffer));
		}
		int family() const {
			return sin6_family;
		}
	};

	//------------------------------------------------------------------------
	struct ip4_stream : ip4 {
		using ip4::ip4;

		static constexpr int domain{ PF_INET };
		static constexpr int type{ SOCK_STREAM };
		static constexpr int protocol{ IPPROTO_TCP };
	};

	//------------------------------------------------------------------------
	struct ip4_datagram : ip4 {
		using ip4::ip4;

		static constexpr int domain{ PF_INET };
		static constexpr int type{ SOCK_DGRAM };
		static constexpr int protocol{ IPPROTO_UDP };
	};

	//------------------------------------------------------------------------
	struct ip6_stream : ip6 {
		using ip6::ip6;

		static constexpr int domain{ PF_INET6 };
		static constexpr int type{ SOCK_STREAM };
		static constexpr int protocol{ IPPROTO_TCP };
	};

	//------------------------------------------------------------------------
	struct ip6_datagram : ip6 {
		using ip6::ip6;

		static constexpr int domain{ PF_INET6 };
		static constexpr int type{ SOCK_STREAM };
		static constexpr int protocol{ IPPROTO_UDP };
	};
} // net
