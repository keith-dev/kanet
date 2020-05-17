#pragma once

#include "address.hpp"

#include <spdlog/spdlog.h>

#include <unistd.h>
#include <sys/socket.h>

#include <memory>
#include <stdexcept>

//----------------------------------------------------------------------------

namespace net {
	//------------------------------------------------------------------------
	class Socket {
		int s_{-1};

	protected:
		int	handle(int s) { return s_ = s; }

	public:
		Socket(int s = -1) : s_(s) {}
		~Socket() { close(); }

		Socket(const Socket& n) = delete;
		Socket& operator=(const Socket& n) = delete;

		Socket(Socket&& s) : s_(s.s_) { s.s_ = -1; }
		Socket& operator=(Socket&& s) {
			if (this != &s) {
				close();

				s_ = s.s_;
				s.s_ = -1;
			}
			return *this;
		}

		int	handle() const { return s_; }

		void close() {
			if (s_ != -1) {
				::close(s_);
				s_ = -1;
			}
		}
	};

	//------------------------------------------------------------------------
	class TCP : public Socket {
	public:
		TCP(int s) : Socket(s) {}
		TCP() = default;
		~TCP() = default;

		std::string recv(size_t nbytes = 4096) {
			std::unique_ptr<char[]> buffer{ new char[nbytes] };

			int ret = ::recv(handle(), buffer.get(), nbytes, 0);
			if (ret == -1)
				throw std::runtime_error{fmt::format("{}() failed: errno={} desc=\"{}\" payload=\"{}\"", "recv", errno, strerror(errno), buffer.get())};

			return { buffer.get(), static_cast<size_t>(ret) };
		}

		size_t send(std::string_view buffer) {
			int ret = ::send(handle(), buffer.data(), buffer.size(), 0);
			if (ret == -1)
				throw std::runtime_error{fmt::format("{}() failed: errno={} desc=\"{}\" payload=\"{}\"", "recv", errno, strerror(errno), buffer)};

			return static_cast<size_t>(ret);
		}
	};

	//------------------------------------------------------------------------
	template <typename ADDR>
	class UDP : public Socket {
	protected:
		UDP() = default;
		~UDP() = default;

//		UDP(const UDP& n) = delete;
//		UDP& operator=(const UDP& n) = delete;

//		UDP(UDP&& n) = default;
//		UDP& operator=(UDP&& n) = default;

	public:
		std::string recvfrom(ADDR& addr) {
			std::unique_ptr<char[]> buffer{ new char[4096] };

			socklen_t len = 0;
			int ret = ::recvfrom(handle(), buffer.get(), 4096, 0, &addr, &len);
			if (ret == -1)
				throw std::runtime_error{fmt::format("{}() failed: errno={} desc=\"{}\" payload=\"{}\"", "recvfrom", errno, strerror(errno), buffer)};
			if (sizeof(addr) != len)
				throw std::runtime_error("address type error");

			return { buffer.get(), static_cast<size_t>(ret) };
		}

		size_t sendto(std::string_view buffer, const ADDR& addr) {
			int ret = ::sendto(handle(), buffer.data(), buffer.size(), 0, addr, addr.size());
			if (ret == -1)
				throw std::runtime_error{fmt::format("{}(\"{}:{}\") failed: errno={} desc=\"{}\" payload=\"{}\"", "sendto", addr.address(), addr.port(), errno, strerror(errno), buffer)};

			return static_cast<size_t>(ret);
		}
	};

	//------------------------------------------------------------------------
	template <typename ADDR>
	class TCPServer : public TCP {
	public:
		TCPServer(ADDR addr) : TCP(::socket(ADDR::domain(), ADDR::type(), ADDR::protocol())) {
			bind(addr);
			listen(16);
		}
		~TCPServer() = default;

//		void socket() {
//			int s = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
//			if (s == -1)
//				throw std::runtime_error{fmt::format("{}() failed: errno={} desc=\"{}\"", "connect", errno, strerror(errno))};
//
//			handle(s);
//		}
//
		void bind(const ADDR& addr) {
			int ret = ::bind(handle(), addr, addr.size());
			if (ret == -1)
				throw std::runtime_error{fmt::format("{}() failed: errno={} desc=\"{}\" address=\"{}:{}\"", "bind", errno, strerror(errno), addr.address(), addr.port())};
		}

		void listen(int depth) {
			int ret = ::listen(handle(), depth);
			if (ret == -1)
				throw std::runtime_error{fmt::format("{}() failed: errno={} desc=\"{}\" depth=\"{}\"", "listen", errno, strerror(errno), depth)};
		}
	};
} // net
