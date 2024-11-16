#ifdef 0
#include "role.hpp"
#include "session.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/http/file_body.hpp>

#include <memory>
#include <string_view>

namespace chat {


class Client {
public:
	Client();

	Client(const Client &)            = delete;
	Client(Client &&)                 = delete;
	Client &operator=(const Client &) = delete;
	Client &operator=(Client &&)      = delete;

	//~Client();

	void connect_to(std::string_view host, std::string_view port
					//, std::string_view request,
					// std::chrono::seconds time_to_leave
	);

	void disconnect();

	/**
	 * \brief Returns true if this client is connected successfully to a server
	 * via a session established
	 */
	[[nodiscard]] bool is_connected();

	/**
	 * \brief Read whole message from the connected server asynchronously.
	 *
	 * \throw None This function does not throw any exceptions.
	 * \param message A string to be sent
	 */
	asio::awaitable<std::string> read();

	/**
	 * \brief Send a message to the connected server asynchronously
	 *
	 * \throw None This function does not throw any exceptions.
	 * \param message A string to be sent
	 */

	asio::awaitable<void> send(std::string_view message);

	asio::awaitable<void> loop();

protected:
	asio::awaitable<void> resolve(std::string_view host, std::string_view port);

private:
	http::request<http::file_body> prepare_get_request(std::string_view host,
													   std::string_view target,
													   int version = 11);

	// A synchronization handler managing the data transfer
	asio::io_context context_;

	asio::ip::tcp::socket socket_;
	// 1 client can only connect 1 user at a time, this connection is
	// managed by a Session
	std::unique_ptr<Session> session_;

	// A thread-safe queue storing the incoming messages from a remote
	// server via a secure session establish before
	// SafeDeque<TextMessageF> received_message_;
};

} // namespace chat
#endif
