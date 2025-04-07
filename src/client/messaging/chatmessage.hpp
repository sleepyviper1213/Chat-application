#pragma once

#include "actor.hpp"
#include "messagebody.hpp"

#include <QtCore/QDateTime>
#include <QtQml/QQmlEngine>

class ChatMessage : public QObject {
	Q_OBJECT

	//	Q_PROPERTY(QObject *board READ board BINDABLE boardBindable)
	Q_PROPERTY(Actor *author READ author WRITE setAuthor NOTIFY authorChanged
				   FINAL REQUIRED)
	Q_PROPERTY(Actor *recipient READ recipient WRITE setRecipient NOTIFY
				   recipientChanged FINAL REQUIRED)
	Q_PROPERTY(QDateTime sentAt READ sentAt WRITE setSentAt NOTIFY sentAtChanged
				   FINAL REQUIRED)
	Q_PROPERTY(MessageBody *body READ body WRITE setBody NOTIFY bodyChanged
				   FINAL REQUIRED)
	// infered attr
	Q_PROPERTY(bool isSentByMe READ isSentByMe CONSTANT FINAL)

	QML_ELEMENT
public:
	explicit ChatMessage(QObject *parent = nullptr);

	//	QObject *board() const;
	//	QBindable<QObject *> boardBindable();

	[[nodiscard]] Actor *author() const;
	void setAuthor(Actor *newAuthor);

	[[nodiscard]] Actor *recipient() const;
	void setRecipient(Actor *newRecipient);

	[[nodiscard]] QDateTime sentAt() const;
	void setSentAt(const QDateTime &newSentAt);

	[[nodiscard]] MessageBody *body() const;
	void setBody(MessageBody *newBody);

	[[nodiscard]]
	bool isSentByMe() const;

signals:
	void authorChanged();

	void recipientChanged();

	void sentAtChanged();

	void bodyChanged();

private:
	// QProperty<QObject *> m_board;
	QPointer<Actor> m_author;
	QPointer<Actor> m_recipient;
	QDateTime m_sentAt;
	MessageBody *m_body;
};
