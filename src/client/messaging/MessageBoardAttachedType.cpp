#include "MessageBoardAttachedType.hpp"

MessageBoardAttachedType::MessageBoardAttachedType(QObject *parent)
	: QObject(parent) {}

bool MessageBoardAttachedType::isExpired() const { return m_isExpired; }

void MessageBoardAttachedType::setExpired(bool newExpired) {
	if (m_isExpired == newExpired) return;
	m_isExpired = newExpired;
	emit expiredChanged();
}
