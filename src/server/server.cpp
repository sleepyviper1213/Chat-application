#include "server.hpp"

#include "certificate.hpp"
#include "room.hpp"
#include "websocket_session.hpp"

namespace chat {
Server::Server(asio::io_context &context, asio::ip::port_type port_num)
	: acceptor_(context,
				tcp::endpoint(asio::ip::address_v4::loopback(), port_num)) {}

void Server::start() noexcept {
	boost::system::error_code error;
	spdlog::info("[Server] Starting at {}", acceptor_.local_endpoint(error));
	if (error) return spdlog::error("[Server] Unable to start! {}", error);

	// Ignore the exception thrown as most errors are handled with error_code
	asio::co_spawn(acceptor_.get_executor(),
				   accept_incoming_connections(),
				   asio::detached);
}

asio::awaitable<void> Server::accept_incoming_connections() noexcept {
	spdlog::info("[Server] Waiting for a client to connect");
	// FIXME: Look for .html file in this directory
	for (Room room;;) {
		auto [err, socket] =
			co_await acceptor_.async_accept(nothrow_awaitable_t{});
		if (err)
			co_return spdlog::error("[Server] Failed to accept due to {}", err);
		spdlog::info("[Server] Connected to client at {}",
					 socket.remote_endpoint(err));

		try {
			auto ssl = load_server_cert();
			// The lifetime of the shared ptr to Session ends immediately once
			// iterating to the next loop. Therefore, its async operations need
			// to call to shared_from_this() to share the ownership of the smart
			// pointer referring to the underlying Session. Be cautious with the
			// unintentional violations with object lifetime
			std::make_shared<websocket_session>(
				websocket::stream<ssl::stream<beast::tcp_stream>>(
					std::move(socket),
					ssl),
				room)
				->start();
		} catch (const boost::system::error_code &ec) {
			spdlog::critical("{}", ec);
			continue;
		}
	}
}

void Server::stop() noexcept {
	spdlog::info("[Server] Stopping server...");
	boost::system::error_code err;
	if (acceptor_.cancel(err); err)
		return spdlog::error("[Server] Stop: {}", err);

	spdlog::info("[Server] Stopped!");
}

} // namespace chat
