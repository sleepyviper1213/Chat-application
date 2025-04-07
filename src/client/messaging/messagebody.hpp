#pragma once


#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVariant>

#include <compare>

// TODO: File, image, something.
class MessageBody : public QObject {
	Q_OBJECT
public:
	static MessageBody fromString(QString text);

	[[nodiscard]] QString text() const;
	void setText(const QString &newText);

	// QString toQString() const;

	// operator QString() const;

	// operator QVariant() const;

	// friend QDataStream &operator<<(QDataStream &out,
	// 							   const MessageBody &message);

	// friend QDataStream &operator>>(QDataStream &in, MessageBody &message);

	// auto operator<=>(const MessageBody &other) const;

	MessageBody(QString text, QObject *parent = nullptr);

signals:
	void textChanged();

private:
	QString m_text;
	Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged FINAL)
};

Q_DECLARE_METATYPE(MessageBody)
