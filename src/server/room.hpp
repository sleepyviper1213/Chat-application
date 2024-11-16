#pragma once

#include "utility.hpp"

#include <mutex>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

namespace chat {
class websocket_session;

class [[nodiscard]] Room {
public:
	/*
	 * \brief Allow a new client entering an session established to read the
	 * most recent messages in the chat room.
	 *
	 * FIXME: Rename
	 */
	bool join(websocket_session *new_client);

	/**
	 * \brief Remove \a participant out of this room
	 *
	 * \note Lifetime of \a participant has not ended yet.
	 */
	void remove(websocket_session *participant);

	/**
	 * \brief Forward \a msg to all of the participants connecting to this room
	 * asynchronously
	 */
	asio::awaitable<void> broadcast(std::string_view msg);

	/**
	 * \brief Forward \a recent_msgs_ to all of the participants connecting to
	 * this room asynchronously
	 */
	asio::awaitable<void>
	load_history(const std::shared_ptr<websocket_session> &a);

	/**
	 * \brief Transfer \a message from client to server's database
	 *
	 * \note Just pretend this function actually send message to a database. I
	 * haven't implemented it yet.
	 */
	void save_to_db(std::string_view message);

private:
	/**
	 * \brief Create a list of all weak pointers representing the sessions,to
	 * send message without holding the mutex:
	 */
	std::vector<std::weak_ptr<websocket_session>> get_weak_ptrs();
	std::mutex mutex_;
	std::unordered_set<websocket_session *> participants_;
	std::vector<std::string> recent_msgs_;
};

} // namespace chat
