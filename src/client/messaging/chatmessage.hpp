#pragma once

#include "actor.hpp"

#include <QQmlEngine>
#include <QtCore>

class ChatMessage : public QObject {
Q_OBJECT

//	Q_PROPERTY(QObject *board READ board BINDABLE boardBindable)
    Q_PROPERTY(Actor *author READ getAuthor WRITE setAuthor NOTIFY authorChanged
                       FINAL REQUIRED)
    Q_PROPERTY(Actor *recipient READ getRecipient WRITE setRecipient NOTIFY
                       recipientChanged FINAL REQUIRED)
    Q_PROPERTY(QDateTime sentAt READ getSentAt WRITE setSentAt NOTIFY
                       sentAtChanged FINAL REQUIRED)
    Q_PROPERTY(QString body READ getBody WRITE setBody NOTIFY bodyChanged FINAL
                       REQUIRED)

    QML_ELEMENT

public:
    explicit ChatMessage(QObject *parent = nullptr);

//	QObject *board() const;
//	QBindable<QObject *> boardBindable();

    [[nodiscard]] Actor *getAuthor() const;

    void setAuthor(Actor *newAuthor);

    [[nodiscard]] Actor *getRecipient() const;

    void setRecipient(Actor *newRecipient);

    [[nodiscard]] QDateTime getSentAt() const;

    void setSentAt(const QDateTime &newSentAt);

    [[nodiscard]] QString getBody() const;

    void setBody(const QString &newBody);

signals:

    void authorChanged();

    void recipientChanged();

    void sentAtChanged();

    void bodyChanged();

private:
    QProperty<QObject *> m_board;
    QPointer<Actor> author;
    QPointer<Actor> recipient;
    QDateTime sentAt;
    QString body;
};
