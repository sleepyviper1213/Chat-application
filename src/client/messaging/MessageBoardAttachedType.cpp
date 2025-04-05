#include "MessageBoardAttachedType.hpp"

MessageBoardAttachedType::MessageBoardAttachedType(QObject *parent)
	: QObject(parent) {}

bool MessageBoardAttachedType::isExpired() const { return expired; }

void MessageBoardAttachedType::setExpired(bool newExpired) {
	if (expired == newExpired) return;
	expired = newExpired;
	emit expiredChanged();
}
