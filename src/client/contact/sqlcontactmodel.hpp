#pragma once

#include <QQmlEngine>
#include <QSqlQueryModel>

class SqlContactModel : public QSqlQueryModel {
	Q_OBJECT
	QML_ELEMENT
public:
	explicit SqlContactModel(QObject *parent = nullptr);

	// Override to provide custom role names
	QHash<int, QByteArray> roleNames() const override;
	QVariant data(const QModelIndex &index,
				  int role = Qt::DisplayRole) const override;

	enum Roles { NameRole = Qt::UserRole + 1, ActorRole };
};
