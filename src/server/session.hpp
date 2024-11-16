#pragma once

#include "participant.hpp"
#include "room.hpp"
#include "utility.hpp"

#include <boost/asio/awaitable.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast.hpp>
#include <boost/url.hpp>

#include <exception>
#include <string_view>

namespace chat {

class Session : public Participant,
				public std::enable_shared_from_this<Session> {
public:
	explicit Session(asio::io_context &context, Room &room);

	explicit Session(tcp::socket &&socket, Room &room);


	void disconnect() noexcept;

	[[nodiscard]] bool is_alive() const;

	/**
	 * \throws boost::system::error_code Thrown on failure
	 */
	void cache(const std::string &msg) override;

private:
	// 1 session establish 1 single socket to 1 remote server
	beast::tcp_stream stream_;

	//  TODO: Is there any way to avoid ref attribute?
	//  NOLINTNEXTLINE(cppcoreguidelines-avoid-const-or-ref-data-members)
	Room &room_;
	// The mesage buffer storing messages that needs sending to a server
	std::deque<std::string> write_msgs_;
};


} // namespace chat
