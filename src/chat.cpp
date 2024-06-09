#include "chat.hpp"

#include "utility.hpp"

#include <fmt/chrono.h>
#include <fmt/core.h>

#include <array>
#include <chrono>
#include <iostream>
#include <string>

namespace chat {
using boost::asio::as_tuple;
using boost::asio::buffer;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::dynamic_buffer;
using boost::asio::steady_timer;
using boost::asio::use_awaitable;
using boost::asio::ip::basic_resolver_entry;

ChatSession::ChatSession(std::string_view local_addr,
						 std::string_view local_port,
						 std::string_view remote_addr,
						 std::string_view remote_port,
						 const std::chrono::seconds &timeout)
	: timeout_(timeout) {
	using tcp::resolver::passive;
	tcp::resolver resolver(ctx_);
	basic_resolver_entry<tcp> listen_endpoint, remote_endpoint;

	listen_endpoint = *resolver.resolve(local_addr, local_port, passive);
	remote_endpoint = *resolver.resolve(remote_addr, remote_port);

	co_spawn(ctx_, sender(remote_endpoint), detached);
	co_spawn(ctx_, receiver(listen_endpoint), detached);

	ctx_.run();
}

awaitable<void> ChatSession::sender(tcp::endpoint remote) {
	for (;;) {
		tcp::socket remote_sock(ctx_);

		if (auto &&[error]
			= co_await remote_sock.async_connect(remote,
												 as_tuple(use_awaitable));
			error) {
			fmt::println("Could not connect to: {} - retrying in {}",
						 remote,
						 timeout_);

			steady_timer timer(co_await boost::asio::this_coro::executor);
			timer.expires_after(timeout_);
			co_await timer.async_wait(use_awaitable);
			continue;
		}

		fmt::println("Connected to: {} ", remote);
		connected_ = true;

		std::string data;
		while (connected_) {
			// Read a string from stdin (non-blocking)
			struct pollfd input[1] = {{.fd = 0, .events = POLLIN}};
			if (poll(input, 1, 100 /* timeout in ms */)) {
				char c;
				while (std::cin.get(c) && c != '\n') data += c;

				data += "\r\n";
			}

			co_await async_write(remote_sock,
								 buffer(data),
								 as_tuple(use_awaitable));

			data.clear();
		}
	}
}

awaitable<void> ChatSession::receiver(tcp::endpoint listen) {
	tcp::acceptor acceptor(ctx_, listen);

	while (true) {
		auto &&[error, client]
			= co_await acceptor.async_accept(as_tuple(use_awaitable));

		if (error) {
			fmt::println(stderr, "Accept failed: {}", error.message());
			continue;
		}

		std::string data;

		while (true) {
			auto &&[error, len]
				= co_await async_read_until(client,
											dynamic_buffer(data),
											boost::regex("\r\n"),
											as_tuple(use_awaitable));

			if (error == boost::asio::error::eof) {
				// remote has disconnected
				connected_ = false;
				break;
			}

			fmt::println("{} > {}", client.remote_endpoint(), data);
			data.clear();
		}
	}
}
} // namespace chat
