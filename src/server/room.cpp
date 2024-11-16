#include "room.hpp"

#include "websocket_session.hpp"

#include <spdlog/spdlog.h>

#include <memory>

namespace chat {

bool Room::join(websocket_session *new_client) {
	const std::lock_guard<std::mutex> a(mutex_);

	const auto [_, insertable] = participants_.insert(new_client);
	return insertable;
}

void Room::remove(websocket_session *participant) {
	const std::lock_guard<std::mutex> _(mutex_);

	participants_.erase(participant);
	spdlog::info("[Room] Removed");
}

std::vector<std::weak_ptr<websocket_session>> Room::get_weak_ptrs() {
	const std::lock_guard<std::mutex> _(mutex_);

	std::vector<std::weak_ptr<websocket_session>> v(participants_.size());
	for (auto *p : participants_) v.emplace_back(p->weak_from_this());

	return v;
}

asio::awaitable<void>
Room::load_history(const std::shared_ptr<websocket_session> &a) {
	if (recent_msgs_.empty()) co_return;

	for (const auto &message : recent_msgs_) co_await a->send(message);

	spdlog::info("[Room] Old messages loaded ");
}

void Room::save_to_db(std::string_view message) {
	recent_msgs_.emplace_back(message);
}

asio::awaitable<void> Room::broadcast(std::string_view message) {
	if (participants_.empty()) co_return;

	// For each weak ptr, acquire a strong pointer if not expired, send the
	// message on that session.
	for (const auto &wp : get_weak_ptrs())
		if (const auto sp = wp.lock()) co_await sp->send(message);

	spdlog::info("[Room] Broadcasted");
}

} // namespace chat
