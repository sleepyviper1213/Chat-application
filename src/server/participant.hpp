#pragma once

#include "utility.hpp"

#include <span>
#include <string>

namespace chat {

/**
 * \brief An interface
 * FIXME: Docs
 */
class Participant {
public:
	virtual ~Participant() = default;

	/**
	 * \brief Forward a single message to the connecting client
	 *
	 * This function is a abstraction that allows calling async_send() as
	 * a normal function. As a coroutine (via co_await keyword) cannot be called
	 * in a non-coroutine function
	 *
	 * \throws Nothing
	 */
	virtual asio::awaitable<void> send(std::string_view message) noexcept = 0;

	/**
	 * \brief Send \a message as a complete message from this server
	 * asynchronously
	 *
	 * \throws Nothing
	 */
	virtual asio::awaitable<void>
	load_history_chat(std::span<const std::string> history) noexcept = 0;
};
} // namespace chat
