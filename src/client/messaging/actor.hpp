#pragma once

#include <QQmlEngine>
#include <QtCore>

class Actor : public QObject {
	Q_OBJECT
	QML_ELEMENT
	Q_PROPERTY(QString firstName READ firstName WRITE setFirstName NOTIFY
				   firstNameChanged FINAL)
	Q_PROPERTY(QString lastName READ lastName WRITE setLastName NOTIFY
				   lastNameChanged FINAL)
	Q_PROPERTY(
		QString email READ email WRITE setEmail NOTIFY emailChanged FINAL)
	Q_PROPERTY(bool isSentByMe READ isSentByMe CONSTANT FINAL)
public:
	Actor(QObject *parent = nullptr);

	Actor(const QString &firstName, QObject *parent = nullptr);

	[[nodiscard]] QString firstName() const;
	void setFirstName(const QString &newName);

	[[nodiscard]] QString lastName() const;
	void setLastName(const QString &newLastName);

	[[nodiscard]] QString email() const;
	void setEmail(const QString &newEmail);
	[[nodiscard]] bool isSentByMe() const;

signals:
	void firstNameChanged();
	void lastNameChanged();
	void emailChanged();


private:
	QString m_firstName;
	QString m_lastName;
	QString m_email;
};
