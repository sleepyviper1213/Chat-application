#include "messagebody.hpp"

#include <utility>

MessageBody MessageBody::fromString(QString text) { return MessageBody{text}; }

const QString &MessageBody::getText() const { return text; }

void MessageBody::setText(const QString &newtext) { text = newtext; }

auto MessageBody::operator<=>(const MessageBody &other) const {
	int cmp = text.compare(other.text);
	if (cmp < 0) return std::strong_ordering::less;
	if (cmp > 0) return std::strong_ordering::greater;
	return std::strong_ordering::equal;
}

QDataStream &operator<<(QDataStream &out, const MessageBody &message) {
	out << "Message(" << message << " ...)";
	return out;
}

QDataStream &operator>>(QDataStream &in, MessageBody &message) {
	in >> message;
	return in;
}

MessageBody::MessageBody(QString text) : text(std::move(text)) {}
