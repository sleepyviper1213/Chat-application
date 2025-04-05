#pragma once
#include "MessageBoardAttachedType.hpp"
#include "chatmessage.hpp"

#include <QQmlEngine>
#include <QtCore>

class MessageBoard : public QObject {
	Q_OBJECT
	QML_ATTACHED(MessageBoardAttachedType)
	Q_CLASSINFO("DefaultProperty", "messages")

	Q_PROPERTY(QQmlListProperty<ChatMessage> messages READ messages)
public:
	static MessageBoardAttachedType *qmlAttachedProperties(QObject *object);
	[[nodiscard]] QQmlListProperty<ChatMessage> messages();

private:
	QList<ChatMessage *> m_messages;
};
