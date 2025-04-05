#include "MessageBoard.hpp"

MessageBoardAttachedType *MessageBoard::qmlAttachedProperties(QObject *object) {
	return new MessageBoardAttachedType(object);
}

QQmlListProperty<ChatMessage> MessageBoard::messages() {
	return QQmlListProperty<ChatMessage>(this, &m_messages);
}
