#include "chatmessage.hpp"

ChatMessage::ChatMessage(QObject *parent)
	: QObject(parent),
	  m_author(new Actor(this)),
	  m_recipient(new Actor(this)) {}

//QObject *ChatMessage::board() const { return m_board.value(); }
//
//QBindable<QObject *> ChatMessage::boardBindable() {
//	return QBindable<QObject *>(&m_board);
//}

Actor *ChatMessage::author() const { return m_author.data(); }

void ChatMessage::setAuthor(Actor *newAuthor) {
	if (m_author == newAuthor) return;
	m_author = newAuthor;
	emit authorChanged();
}

Actor *ChatMessage::recipient() const { return m_recipient.data(); }

void ChatMessage::setRecipient(Actor *newRecipient) {
	if (m_recipient == newRecipient) return;
	m_recipient = newRecipient;
	emit recipientChanged();
}

QDateTime ChatMessage::sentAt() const { return m_sentAt; }

void ChatMessage::setSentAt(const QDateTime &newSentAt) {
	if (m_sentAt == newSentAt) return;
	m_sentAt = newSentAt;
	emit sentAtChanged();
}

MessageBody *ChatMessage::body() const { return m_body; }

void ChatMessage::setBody(MessageBody *newBody) {
	if (m_body == newBody) return;
	m_body = newBody;
	emit bodyChanged();
}

bool ChatMessage::isSentByMe() const {
	if (!m_author) {
		qFatal() << "Pointer to Author is NULL";
		return false;
	}
	return m_author->firstName() == "Me";
}
