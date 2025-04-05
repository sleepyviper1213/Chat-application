#pragma once

#include "../../../Qt/6.7.2/macos/lib/QtCore.framework/Headers/QtCore"

#include <compare>

// TODO: File, image, something.
class MessageBody {
public:
	static MessageBody fromString(QString text);

	[[nodiscard]] const QString &getText() const;
	void setText(const QString &newName);

	QString toQString() const { return text; }

	operator QString() const { return text; }

	operator QVariant() const { return QVariant(text); }

	friend QDataStream &operator<<(QDataStream &out,
								   const MessageBody &message);

	friend QDataStream &operator>>(QDataStream &in, MessageBody &message);

	auto operator<=>(const MessageBody &other) const;

protected:
	explicit MessageBody(QString text);

private:
	QString text;
};

Q_DECLARE_METATYPE(MessageBody)
