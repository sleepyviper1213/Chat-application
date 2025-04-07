#include "MessageBoard.hpp"

#include <QtCore/qlogging.h>

MessageBoardAttachedType *MessageBoard::qmlAttachedProperties(QObject *object) {
	return new MessageBoardAttachedType(object);
}

void MessageBoard::refresh() { qDebug() << "Called the C++ slot"; }

QQmlListProperty<ChatMessage> MessageBoard::messages() {
	return QQmlListProperty<ChatMessage>(this,
										 &m_messages,
										 &MessageBoard::append_message,
										 &MessageBoard::count_messages,
										 &MessageBoard::at_message,
										 &MessageBoard::clear_messages);
}

bool MessageBoard::postMessage(const QString &msg) {
	qDebug() << "Called the C++ method with" << msg;
	return true;
}

void MessageBoard::append_message(QQmlListProperty<ChatMessage> *list,
								  ChatMessage *msg) {
	if (!msg) {
		qFatal() << "Pointer to message is NULL";
		return;
	}
	MessageBoard *msgBoard = qobject_cast<MessageBoard *>(list->object);
	if (!msgBoard) {
		qFatal() << "Invalid cast from QML property";
		return;
	}
	msgBoard->m_messages.append(msg);
}

qsizetype MessageBoard::count_messages(QQmlListProperty<ChatMessage> *list) {
	MessageBoard *msgBoard = qobject_cast<MessageBoard *>(list->object);
	return msgBoard ? msgBoard->m_messages.size() : 0;
}

ChatMessage *MessageBoard::at_message(QQmlListProperty<ChatMessage> *list,
									  qsizetype index) {
	MessageBoard *msgBoard = qobject_cast<MessageBoard *>(list->object);
	if (!msgBoard) {
		qFatal() << "Invalid cast from QML property";
		return nullptr;
	}
	if (index < 0 || index >= msgBoard->m_messages.size()) {
		qFatal() << "Index is out of range";
		return nullptr;
	}
	return msgBoard->m_messages.at(index);
}

void MessageBoard::clear_messages(QQmlListProperty<ChatMessage> *list) {
	MessageBoard *msgBoard = qobject_cast<MessageBoard *>(list->object);
	if (!msgBoard) {
		qFatal() << "Invalid cast from QML property";
		return;
	}
	msgBoard->m_messages.clear();
}
