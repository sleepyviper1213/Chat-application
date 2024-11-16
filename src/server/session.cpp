#include "session.hpp"

// #include "websocket_session.hpp"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <spdlog/spdlog.h>

#include <cctype>
#include <utility>

namespace chat {

Session::Session(asio::io_context &context, Room &room)
	: stream_(context), room_(room) {}

Session::Session(asio::ip::tcp::socket &&socket, Room &room)
	: stream_(std::move(socket)), room_(room) {}

void Session::disconnect() noexcept {
	if (!is_alive()) {
		spdlog::info("[Session] Closed already!");
		return;
	}
	spdlog::info("[Session] Closing socket...");
	room_.remove(shared_from_this());
	boost::system::error_code error;
	stream_.socket().shutdown(tcp::socket::shutdown_both, error);
	stream_.socket().close(error);
	if (error) {
		spdlog::error("[Session] Unable to close socket: {}", error);
		return;
	}
	stream_.cancel();
	// request_timer_.cancel();

	spdlog::info("[Session] Closed!");
}

bool Session::is_alive() const { return stream_.socket().is_open(); }

// FIXME: A bug processing bytes when using those methods of read_msg
asio::awaitable<void> Session::listen_for_requests() noexcept {
	// Assume this is a long running task. It is extremely hard to put a
	// timeout, therefore, better to check it every few seconds.
	beast::flat_buffer read_msg;
	const auto [err, bytes_read] =
		co_await http::async_read(stream_,
								  read_msg,
								  parser_,
								  nothrow_awaitable_t{});
	if (err) {
		spdlog::error("[Session] Read: {}", err);
		disconnect();
		co_return;
	}
	spdlog::info("[Session] Received {} bytes", bytes_read);
	// move it to the read buffer
	read_msg.commit(bytes_read);
	// write the read message to all participants in the room
	const auto wbuf = read_msg.data();
	room_.broadcast(beast::buffers_to_string(read_msg.data()));
	// remove parsed bytes from the buffer
	read_msg.consume(bytes_read);
	// See if it is a WebSocket Upgrade request
	// if (websocket::is_upgrade(parser_.get())) {
	// The ownership of the socket and HTTP request has been transferred to
	// WebSocket
	// std::make_shared<websocket_session>(stream_.release_socket(), room_)
	//->run(parser_.release());
	// co_return;
	//}

	co_await send_response();
}

asio::awaitable<void> Session::send_response() noexcept {
	// if (is_alive()) {
	//     // The error returned by timer's async_wait() is intentionally left
	//     // unchecked. If 'Operation canceled' is thrown, it indicates the
	//     // asynchronous wait operation is canceled before the timer expires,
	//     // aka explicitly cancelled using cancel_one() in deliver()
	//     // function. That's when the  operation it was guarding completes
	//     // successfully. Do not remove this line, it's a timeout logic.
	//     [[maybe_unused]] auto _ =
	//         co_await stream_.socket().async_wait(tcp::socket::wait_read,
	//                                              nothrow_awaitable_t{});
	//     co_return;
	// }
	http::message_generator msg =
		handle_request(room_.doc_root(), parser_.release());
	const bool keep_alive = msg.keep_alive();
	const auto [err, bytes_written] =
		co_await beast::async_write(stream_,
									std::move(msg),
									nothrow_awaitable_t{});
	if (err) {
		spdlog::error("[Session] Write: {}", err);
		disconnect();
		co_return;
	}
	spdlog::info("[Session] Sent {} bytes", bytes_written);

	if (!keep_alive) stream_.socket().shutdown(tcp::socket::shutdown_send);
}

void Session::cache(const std::string &msg) {
	spdlog::debug("[Session] Messaged is cached");
	write_msgs_.push_back(msg);
	//  request_timer_.cancel_one();
}

} // namespace chat
