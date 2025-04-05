// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "sqlcontactmodel.hpp"

#include "../messaging/actor.hpp"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

void prepareContactsDatabase() {
	using namespace Qt::StringLiterals;
	QSqlQuery query;
	if (!query.exec(u"CREATE TABLE IF NOT EXISTS 'Contacts' ("
					"   'name' TEXT NOT NULL,"
					"   PRIMARY KEY(name)"
					")"_s)) {
		qFatal("Failed to query database: %s",
			   qPrintable(query.lastError().text()));
	} else qInfo() << "Contact databases prepared";
}

void insertContact(const QString &name) {
	using namespace Qt::StringLiterals;

	QSqlQuery query;
	query.prepare(u"INSERT INTO Contacts VALUES (:name)"_s);
	query.bindValue(":name", name);

	if (!query.exec()) {
		qWarning() << "Failed to insert conversation:"
				   << query.lastError().text();
	} else qInfo() << "Contact inserted";
}

static void createTable() {
	// The table already exists; we don't need to do anything.
	using namespace Qt::StringLiterals;
	if (QSqlDatabase::database().tables().contains(u"Contacts"_s)) return;

	prepareContactsDatabase();

	insertContact("Albert Einstein");
	insertContact("Ernest Hemingway");
	insertContact("Hans Gude");
}

SqlContactModel::SqlContactModel(QObject *parent) : QSqlQueryModel(parent) {
	createTable();

	QSqlQuery query;
	if (!query.exec("SELECT * FROM Contacts"))
		qFatal("Contacts SELECT query failed: %s",
			   qPrintable(query.lastError().text()));

	setQuery(std::move(query));
	if (lastError().isValid())
		qFatal("Cannot set query on SqlContactModel: %s",
			   qPrintable(lastError().text()));
}

QHash<int, QByteArray> SqlContactModel::roleNames() const {
	QHash<int, QByteArray> roles;
	roles[NameRole]  = "name";
	roles[ActorRole] = "actor";
	return roles;
}

QVariant SqlContactModel::data(const QModelIndex &index, int role) const {
	if (!index.isValid()) return QVariant();

	if (role == NameRole) {
		return QSqlQueryModel::data(index, Qt::DisplayRole); // 'name' column
	} else if (role == ActorRole) {
		QString name = QSqlQueryModel::data(index, Qt::DisplayRole).toString();
		Actor *actor =
			new Actor(name,
					  const_cast<SqlContactModel *>(this)); // Parent to model
		return QVariant::fromValue(actor);
	}
	return QVariant();
}
