#pragma once

#include "participant.hpp"
#include "room.hpp"

#include <boost/beast/core/tcp_stream.hpp>

#include <memory>
#include <string_view>

namespace chat {
/**
 * \brief Represents an active WebSocket connection to the server
 */
class [[nodiscard]] websocket_session final
	: public Participant,
	  public std::enable_shared_from_this<websocket_session> {
public:
	websocket_session(
		websocket::stream<ssl::stream<beast::tcp_stream>> &&socket, Room &room);
	~websocket_session() override;

	/**
	 * \brief Perform the asynchronous WebSocket handshake with the server role
	 */
	void start() noexcept;

	/**
	 * \copydoc Participant::load_history_chat
	 */
	asio::awaitable<void>
	load_history_chat(std::span<const std::string> msg) noexcept override;

	/**
	 * \copydoc Participant::send
	 */
	asio::awaitable<void> send(std::string_view message) noexcept override;

private:
	/**
	 * \brief Read a complete message sent to this server asynchronously.
	 * \note Message is not chunked or split like being read by
	 * async_read_some()
	 *
	 * TODO: Only plain text is supported, file transfer will be implemented
	 * later
	 * \throws Nothing
	 */
	asio::awaitable<void> listen_for_incoming_message() noexcept;

	/**
	 * \brief Perform the handshake asynchronously. Once connected, load
	 * previously transmitted messages, and keep listening for new message to
	 * enqueue.
	 *
	 * \throws Nothing
	 */
	asio::awaitable<void> websocket_handshake() noexcept;

	asio::awaitable<void> ssl_handshake() noexcept;
	/**
	 * \brief Remove this session from the list of active sessions
	 */
	void disconnect() noexcept;

	websocket::stream<ssl::stream<beast::tcp_stream>> ws_;

	// FIXME: A ref here is a code smell?
	Room &room_;
};
} // namespace chat
