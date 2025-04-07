#include "sqlconversationmodel.hpp"

#include "messagebody.hpp"
#include "messaging/MessageBoard.hpp"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

#include <QDateTime>
#include <QDebug>

static const char *const conversationsTableName = "Conversations";

static void createTable() {
	if (QSqlDatabase::database().tables().contains(conversationsTableName))
		return;
	using namespace Qt::StringLiterals;
	QSqlQuery query;
	if (!query.exec(
			u"CREATE TABLE IF NOT EXISTS 'Conversations' ("
			"'author' TEXT NOT NULL,"
			"'recipient' TEXT NOT NULL,"
			"'timestamp' TEXT NOT NULL,"
			"'message' TEXT NOT NULL,"
			"FOREIGN KEY('author') REFERENCES Contacts ( name ),"
			"FOREIGN KEY('recipient') REFERENCES Contacts ( name ))"_s)) {
		qFatal("Failed to create table: %s",
			   qPrintable(query.lastError().text()));
		return;
	}
	// Add test data
	query.exec(u"INSERT INTO Conversations (author, recipient, timestamp, "
			   u"message) VALUES "
			   "('Albert Einstein', 'Me', '2025-04-03T10:00:00', 'Hello!')"_s);
	query.exec(
		u"INSERT INTO Conversations (author, recipient, timestamp, message) "
		"VALUES ('Me', 'Albert Einstein', '2025-04-03T10:01:00', 'Hi there!')"_s);
}

SqlConversationModel::SqlConversationModel(QObject *parent)
	: QSqlTableModel(parent) {
	createTable();

	setTable(conversationsTableName);
	setSort(2, Qt::DescendingOrder);
	setEditStrategy(QSqlTableModel::OnManualSubmit);
}

Actor *SqlConversationModel::getRecipient() const {
	return m_recipient.data(); // Return raw pointer from QPointer
}

void SqlConversationModel::setRecipient(Actor *recipient) {
	if (recipient == m_recipient.data()) return;

	m_recipient =
		recipient; // Assign raw pointer to QPointer (implicit conversion)

	const QString filterString =
		QString::fromLatin1("(recipient = '%1' AND author = 'Me') OR "
							"(recipient = 'Me' AND author='%1')")
			.arg(m_recipient->firstName());
	QSqlTableModel::setFilter(filterString);
	QSqlTableModel::select();

	emit recipientChanged();
}

QVariant SqlConversationModel::data(const QModelIndex &index, int role) const {
	if (!index.isValid()) return QVariant();

	if (role == Qt::UserRole + 1) {
		const QSqlRecord record = this->record(index.row());
		auto *message           = new ChatMessage(nullptr);
		message->setRecipient(m_recipient);
		message->setBody(
			new MessageBody(record.value("message").toString(), message));
		message->setSentAt(record.value("timestamp").toDateTime());
		message->setAuthor(
			new Actor(record.value("author").toString(), message));

		return QVariant::fromValue(message);
	}
	return QSqlTableModel::data(index, role); // Fallback for other roles
}

QHash<int, QByteArray> SqlConversationModel::roleNames() const {
	QHash<int, QByteArray> names;
	names[Qt::UserRole + 1] = "message"; // Expose as 'message' in QML
	return names;
}

void SqlConversationModel::sendMessageBody(const QString &body) {
	ChatMessage chat;
	chat.setAuthor(new Actor("Me", &chat));
	chat.setRecipient(m_recipient);
	chat.setSentAt(QDateTime::currentDateTime());
	chat.setBody(new MessageBody(body, &chat));
	sendMessage(chat);
}

void SqlConversationModel::sendMessage(const ChatMessage &chat) {
	QSqlRecord newRecord = QSqlTableModel::record();
	newRecord.setValue("author", chat.author()->firstName());
	newRecord.setValue("recipient", chat.recipient()->firstName());
	newRecord.setValue("timestamp", chat.sentAt().toString(Qt::ISODate));
	newRecord.setValue("message", chat.body()->text());
	if (!QSqlTableModel::insertRecord(QSqlTableModel::rowCount(), newRecord)) {
		qWarning() << "Failed to send message:" << lastError().text();
		return;
	}
	MessageBoardAttachedType *attached =
		qobject_cast<MessageBoardAttachedType *>(
			qmlAttachedPropertiesObject<MessageBoard>(&chat));
	if (attached) {
		qDebug() << "Value of MessageBoard.expired:" << attached->isExpired();
		emit attached->published();
	}
	QSqlTableModel::submitAll();
}
