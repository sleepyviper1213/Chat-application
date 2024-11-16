#pragma once
#include "utility.hpp"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/steady_timer.hpp>

namespace chat {
class watchdog {
public:
	watchdog(asio::any_io_executor exec, std::chrono::milliseconds interval)
		: tim(exec, interval), interval(interval) {}

	void rewatch() {
		tim.expires_after(interval);
		asio::co_spawn(tim.get_executor(), reset(), asio::detached);
	}

	asio::cancellation_signal &get_cancel() { return cancel; }

private:
	asio::steady_timer tim;
	std::chrono::milliseconds interval;
	asio::cancellation_signal cancel;

	asio::awaitable<void> reset() {
		auto [ec] = co_await tim.async_wait(nothrow_awaitable_t{});
		if (!ec) cancel.emit(asio::cancellation_type::terminal);
	}
};
} // namespace chat
