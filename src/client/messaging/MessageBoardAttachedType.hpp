#pragma once
#include <QQmlEngine>
#include <QtCore>

class MessageBoardAttachedType : public QObject {
	Q_OBJECT
	Q_PROPERTY(bool expired READ isExpired WRITE setExpired NOTIFY
				   expiredChanged FINAL)
	QML_ANONYMOUS
public:
	explicit MessageBoardAttachedType(QObject *parent = nullptr);
	[[nodiscard]] bool isExpired() const;
	void setExpired(bool newExpired);

signals:
	void published();

	void expiredChanged();

private:
	bool expired;
};
