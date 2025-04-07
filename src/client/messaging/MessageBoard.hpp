#pragma once
#include "MessageBoardAttachedType.hpp"
#include "chatmessage.hpp"

#include <QtQml/QQmlEngine>

class MessageBoard : public QObject {
	Q_OBJECT
	QML_ATTACHED(MessageBoardAttachedType)
	Q_CLASSINFO("DefaultProperty", "messages")

	Q_PROPERTY(QQmlListProperty<ChatMessage> messages READ messages)
public:
	[[nodiscard]] QQmlListProperty<ChatMessage> messages();

	[[nodiscard]]Q_INVOKABLE bool postMessage(const QString &msg);

signals:
	void newMessagePosted(const QString &subject);

public slots:

	void refresh();

private:
	static MessageBoardAttachedType *qmlAttachedProperties(QObject *object);
	static void append_message(QQmlListProperty<ChatMessage> *list,
							   ChatMessage *msg);
	static qsizetype count_messages(QQmlListProperty<ChatMessage> *list);
	static ChatMessage *at_message(QQmlListProperty<ChatMessage> *list,
								   qsizetype index);
	static void clear_messages(QQmlListProperty<ChatMessage> *list);

	QList<ChatMessage *> m_messages;
};
