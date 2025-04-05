#pragma once
#include "utility.hpp"

#include <spdlog/spdlog.h>

namespace chat {

/**
 * \brief A simple TCP server with SSL certificates required
 */
class [[nodiscard]] Server {
public:
	/**
	 * \brief Construct the server, but not open for connection yet
	 *
	 * \note When constructed this way, it is necessary to call \a
	 * open_connection_from() to start listening
	 */
	explicit Server(tcp::acceptor acceptor) : acceptor_(std::move(acceptor)) {}

	/**
	 * \brief Construct the server listening to port \a port_num one the local
	 * host using IPv4 address
	 */
	Server(asio::io_context &io, asio::ip::port_type port_num);

	/**
	 * \brief Accept the connection request asynchronously. Once accepted, try
	 * to load SSL certificates in this source directory and start a Websocket
	 * connection/
	 */
	void start() noexcept;

	/**
	 * \brief Provide a service to which a client can request a connection using
	 * IP address and port number from a \a service endpoint
	 *
	 * \tparam Endpoint
	 * \param service An endpoint to which a client can connect
	 */
	static Server
	open_connection_from(asio::io_context &context,
						 const requirement::Endpoint auto &service) {
		spdlog::info("[Server] Opening connection");

		tcp::acceptor acceptor_(context);
		acceptor_.open(service.protocol());
		acceptor_.set_option(asio::socket_base::reuse_address(true));

		acceptor_.bind(service);
		acceptor_.listen(asio::socket_base::max_listen_connections);
		return Server{std::move(acceptor_)};
	}

	/**
	 * \brief Cancel all runnning services before terminating.
	 */
	void stop() noexcept;

private:
	asio::awaitable<void> accept_incoming_connections() noexcept;

	tcp::acceptor acceptor_;
};
} // namespace chat
