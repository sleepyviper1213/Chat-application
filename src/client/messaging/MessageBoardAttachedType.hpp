#pragma once
#include <QtQml/QQmlEngine>

class MessageBoardAttachedType : public QObject {
	Q_OBJECT
	Q_PROPERTY(bool isExpired READ isExpired WRITE setExpired NOTIFY
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
	bool m_isExpired;
};
