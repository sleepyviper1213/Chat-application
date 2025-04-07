#pragma once

#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>

void connectToDatabase() {
	QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
	if (!database.isValid())
		qFatal() << "Cannot add database: " << database.lastError().text();

	const QDir writeDir =
		QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	if (!writeDir.mkpath("."))
		qFatal("Failed to create writable directory at %s",
			   qPrintable(writeDir.absolutePath()));

	// Ensure that we have a writable location on all devices.
	const QString fileName = writeDir.absolutePath() + "/chat-database.sqlite3";
	// When using the SQLite driver, open() will create the SQLite database if
	// it doesn't exist.
	database.setDatabaseName(fileName);
	if (!database.open()) {
		qFatal() << "Cannot open database: " << database.lastError().text();
		QFile::remove(fileName);
	}
}
