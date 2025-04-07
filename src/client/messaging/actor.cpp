#include "actor.hpp"

Actor::Actor(QObject *parent) : QObject(parent) {}

Actor::Actor(const QString &firstName, QObject *parent)
	: QObject(parent), m_firstName(firstName) {}

QString Actor::firstName() const { return m_firstName; }

void Actor::setFirstName(const QString &newName) {
	if (m_firstName == newName) return;
	m_firstName = newName;
	emit firstNameChanged();
}

QString Actor::lastName() const { return m_lastName; }

void Actor::setLastName(const QString &newLastName) {
	if (m_lastName == newLastName) return;
	m_lastName = newLastName;
	emit lastNameChanged();
}

QString Actor::email() const { return m_email; }

void Actor::setEmail(const QString &newEmail) {
	if (m_email == newEmail) return;
	m_email = newEmail;
	emit emailChanged();
}
