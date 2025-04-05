#include "chatmessage.hpp"

ChatMessage::ChatMessage(QObject *parent)
        : QObject(parent),
          author(new Actor(parent)),
          recipient(new Actor(parent)) {}

//QObject *ChatMessage::board() const { return m_board.value(); }
//
//QBindable<QObject *> ChatMessage::boardBindable() {
//	return QBindable<QObject *>(&m_board);
//}

Actor *ChatMessage::getAuthor() const { return author.data(); }

void ChatMessage::setAuthor(Actor *newAuthor) {
    if (author == newAuthor)
        return;
    author = newAuthor;
    emit authorChanged();
}

Actor *ChatMessage::getRecipient() const { return recipient.data(); }

void ChatMessage::setRecipient(Actor *newRecipient) {
    if (recipient == newRecipient)
        return;
    recipient = newRecipient;
    emit recipientChanged();
}

QDateTime ChatMessage::getSentAt() const { return sentAt; }

void ChatMessage::setSentAt(const QDateTime &newSentAt) {
    if (sentAt == newSentAt) return;
    sentAt = newSentAt;
    emit sentAtChanged();
}

QString ChatMessage::getBody() const { return body; }

void ChatMessage::setBody(const QString &newBody) {
    if (body == newBody) return;
    body = newBody;
    emit bodyChanged();
}
