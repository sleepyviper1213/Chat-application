#ifdef LOCK
#include "client.hpp"

#include <boost/asio/buffers_iterator.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http/field.hpp>
#include <boost/beast/version.hpp>
#include <spdlog/spdlog.h>

#include <chrono>
#include <coroutine>
#include <iostream>
#include <string>
#include <string_view>

namespace chat {
Client::Client() : socket_(context_) {}

// Client::~Client() { disconnect(); }

void Client::connect_to(std::string_view host, std::string_view port
						//, std::string_view request,
						// std::chrono::seconds time_to_leave
) {
	spdlog::info("[Client] Connecting to {}:{}", host, port);
	asio::co_spawn(context_, resolve(host, port), asio::detached);
	context_.run();
}

void Client::disconnect() {
	if (!is_connected()) return;
	spdlog::info("[Client] Trying to disconnect");
	session_->disconnect();
	spdlog::info("[Client] Disconnected!");
}

bool Client::is_connected() {
	if (!session_) {
		spdlog::error("[Client] No session created to close!");
		return false;
	}
	return session_->is_alive();
}

asio::awaitable<std::string> Client::read() {
	asio::streambuf received_buf;
	const auto bytes_read =
		co_await asio::async_read(socket_, received_buf, nothrow_awaitable_t{});
	bytes_read.assume_value();
	if (bytes_read) {
		spdlog::error("Error occured while reading: {}", bytes_read.error());
		co_return std::string{};
	}
	std::string data(asio::buffers_begin(received_buf.data()),
					 asio::buffers_end(received_buf.data()));
	co_return data;
}

asio::awaitable<void> Client::send(std::string_view message) {
	const auto bytes_sent = co_await asio::async_write(socket_,
													   asio::buffer(message),
													   nothrow_awaitable_t{});
	if (bytes_sent) {
		spdlog::error("Error occured while sending: {}", bytes_sent.error());
		co_return;
	}
	spdlog::info("Sent {} bytes", bytes_sent.value());
}

asio::awaitable<void> Client::resolve(std::string_view host,
									  std::string_view port) {
	asio::ip::tcp::resolver resolver(co_await asio::this_coro::executor);
	const auto endpoints =
		co_await resolver.async_resolve(host, port, nothrow_awaitable_t{});
	if (endpoints) {
		spdlog::error("[Client] Failed to resolve due to {}",
					  endpoints.error());
		co_return;
	}
	spdlog::info("[Client] Setting up a session");

	const auto server = co_await asio::async_connect(socket_,
													 endpoints.value(),
													 nothrow_awaitable_t{});
	if (server) {
		spdlog::error("Can't request a connection to service!");
		co_return;
	}
	spdlog::info("[Client] Connected to {} successfully", server.value());
	session_ = std::make_unique<Session>(std::move(socket_), Role::CLIENT);
	co_await loop();
}

asio::awaitable<void> Client::loop() {
	std::string s;
	while (true) {
		std::getline(std::cin, s);
		co_await session_->send_request(s);
	}
}

http::request<http::file_body>
Client::prepare_get_request(std::string_view host, std::string_view target,
							int version) {
	http::request<http::file_body> req(http::verb::get, target, version);
	req.set(http::field::host, host);
	req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
	req.keep_alive(false);
	req.prepare_payload();

	return req;
}

} // namespace chat
#endif
