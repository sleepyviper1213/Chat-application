#pragma once

#include "../messaging/actor.hpp"
#include "../messaging/chatmessage.hpp"

#include <QQmlEngine>
#include <QSqlTableModel>

class SqlConversationModel : public QSqlTableModel {
	Q_OBJECT
	QML_ELEMENT
	Q_PROPERTY(Actor *recipient READ getRecipient WRITE setRecipient NOTIFY
				   recipientChanged FINAL REQUIRED)

public:
	explicit SqlConversationModel(QObject *parent = nullptr);

	[[nodiscard]] Actor *getRecipient() const;
	void setRecipient(Actor *recipient);

	[[nodiscard]] QVariant data(const QModelIndex &index,
								int role) const override;
	[[nodiscard]] QHash<int, QByteArray> roleNames() const override;

	Q_INVOKABLE void sendMessageBody(const QString &message);
	Q_INVOKABLE void sendMessage(const ChatMessage &message);

signals:
	void recipientChanged();

private:
	QPointer<Actor> m_recipient; // Still use QPointer internally
};
