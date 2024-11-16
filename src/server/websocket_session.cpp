#include "websocket_session.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace chat {

websocket_session::websocket_session(
	websocket::stream<ssl::stream<beast::tcp_stream>> &&socket, Room &room)
	: ws_(std::move(socket)), room_(room) {}

void websocket_session::disconnect() noexcept {
	beast::beast_close_socket(beast::get_lowest_layer(ws_));
	room_.remove(this);
}

asio::awaitable<void> websocket_session::ssl_handshake() noexcept {
	// Set the timeout.
	beast::get_lowest_layer(ws_).expires_after(std::chrono::seconds(30));
	const auto [err] =
		co_await ws_.next_layer().async_handshake(ssl::stream_base::server,
												  nothrow_awaitable_t{});
	if (err) co_return spdlog::error("[Session] SSL error: {}", err);
	co_await websocket_handshake();
}

void websocket_session::start() noexcept {
	// shared_from_this() ensures the Session object is alive during the
	// execution of listen_for_request() and send_request(). It prevents
	// accessing Session that might be destructed before its operation
	// starting them completes. Could lead to memory leak if object lifetime
	// is messed, i.e, circular references. Do not try to replace this
	// lambda expression by a direct call to async function.
	asio::co_spawn(
		ws_.get_executor(),
		[self = shared_from_this()] { return self->ssl_handshake(); },
		asio::detached);
}

asio::awaitable<void> websocket_session::websocket_handshake() noexcept {
	// Turn off the timeout on the tcp_stream, because
	// the websocket stream has its own timeout system.
	beast::get_lowest_layer(ws_).expires_never();
	// Set suggested timeout for the websocket-based server
	ws_.set_option(
		websocket::stream_base::timeout::suggested(beast::role_type::server));

	// Set a decorator to change the Server field of the handshake
	ws_.set_option(
		websocket::stream_base::decorator([](websocket::response_type &res) {
			res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
		}));

	const auto [err] = co_await ws_.async_accept(nothrow_awaitable_t{});
	if (err) co_return spdlog::error("Unable to accept request");

	if (bool x = room_.join(this); !x)
		co_return spdlog::error("[Room] Participant joined");
	co_await room_.load_history(shared_from_this());
	co_await listen_for_incoming_message();
}

asio::awaitable<void>
websocket_session::listen_for_incoming_message() noexcept {
	for (beast::flat_buffer buffer;;) {
		const auto [err, _] =
			co_await ws_.async_read(buffer, nothrow_awaitable_t{});
		// Websocket connection closed gracefully. Ignore it
		if (err == websocket::error::closed)
			co_return spdlog::info("[Session] {}", err);
		else if (err) co_return spdlog::error("[Session] Read: {}", err);

		assert(ws_.is_message_done());
		ws_.text(ws_.got_text());
		spdlog::info("[Session] Received");

		const auto received_msg = beast::buffers_to_string(buffer.cdata());
		// Clear the buffer
		buffer.consume(buffer.size());

		if (!nlohmann::json::accept(received_msg)) {
			spdlog::error("[Session] Invalid json");
			continue;
		}
		room_.save_to_db(received_msg);
		// Send to all users connecting to this room
		co_await room_.broadcast(received_msg);
	}
}

asio::awaitable<void> websocket_session::load_history_chat(
	std::span<const std::string> history) noexcept {
	assert(ws_.is_open());

	for (const auto &old_msg : history) co_await send(old_msg);
	spdlog::info("[Session] History chat loaded");
}

asio::awaitable<void>
websocket_session::send(std::string_view message) noexcept {
	if (const auto [err, _] = co_await ws_.async_write(asio::buffer(message),
													   nothrow_awaitable_t{});
		err) {
		spdlog::error("[Session] Write: {}", err);
		// TODO: Handle if writing error occured
		co_return disconnect();
	}
	spdlog::info("[Session] Forwarded");
}

websocket_session::~websocket_session() { disconnect(); }
} // namespace chat
