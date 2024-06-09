#pragma once

#include <boost/asio.hpp>
#include <boost/asio/as_tuple.hpp>
#include <boost/regex.hpp>

#include <chrono>
#include <string_view>

namespace chat {
using boost::asio::awaitable;
using boost::asio::io_context;
using boost::asio::ip::tcp;
using namespace std::chrono_literals;

class ChatSession {
public:
	ChatSession(std::string_view local_addr, std::string_view local_port,
				std::string_view remote_addr, std::string_view remote_port,
				const std::chrono::seconds &timeout = 1s);

private:
	awaitable<void> sender(tcp::endpoint remote);

	awaitable<void> receiver(tcp::endpoint listen);

	// Member variables (shared between coroutines)
	io_context ctx_;

	std::chrono::seconds timeout_;
	bool connected_ = false;
};

} // namespace chat
