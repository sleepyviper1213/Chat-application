#pragma once
#include "utility.hpp"

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <spdlog/spdlog.h>

namespace chat {

class ServerBase {
private:
	/**
	 * \brief The server will accept the connection request asynchronously at
	 * localhost
	 */
	asio::awaitable<void> accept_incoming_connections() {
		spdlog::info("[Server] Waiting for a client to connect");
		// FIXME: Look for .html file in this directory
		Room room("/Users/truong/Chat-application/src/");
		for (;;) {
			auto [err, socket] =
				co_await acceptor_.async_accept(nothrow_awaitable_t{});
			if (err) {
				spdlog::error("[Server] Failed to accept socket due to {}",
							  err);
				co_return;
			}
			spdlog::info("[Server] Connected to client at {}",
						 socket.remote_endpoint(err));
			auto session = std::make_shared<Session>(std::move(socket), room);
			session->start();
		}
	}

	asio::ip::tcp::acceptor acceptor_;
	// FIXME: Vector of shared_ptr? or raw ptr
	// std::vector<std::shared_ptr<Session>> sessions_;
	// SafeDeque<TextMessageF> received_message_;
};
} // namespace chat
