#include "messagebody.hpp"

#include <utility>

MessageBody MessageBody::fromString(QString text) { return MessageBody{text}; }

// auto MessageBody::operator<=>(const MessageBody &other) const {
// 	int cmp = m_text.compare(other.m_text);
// 	if (cmp < 0) return std::strong_ordering::less;
// 	if (cmp > 0) return std::strong_ordering::greater;
// 	return std::strong_ordering::equal;
// }

// QDataStream &operator<<(QDataStream &out, const MessageBody &message) {
// 	out << "Message(" << message << " ...)";
// 	return out;
// }

// QDataStream &operator>>(QDataStream &in, MessageBody &message) {
// 	in >> message;
// 	return in;
// }

MessageBody::MessageBody(QString text, QObject *parent)
	: QObject(parent), m_text(std::move(text)) {}

void MessageBody::setText(const QString &newText) {
	if (m_text == newText) return;
	m_text = newText;
	emit textChanged();
}

QString MessageBody::text() const { return m_text; }

// QString MessageBody::toQString() const { return m_text; }

// MessageBody::operator QString() const { return m_text; }

// MessageBody::operator QVariant() const { return QVariant(m_text); }
