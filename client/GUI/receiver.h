#pragma once
#ifndef RECEIVER_H
#define RECEIVER_H

#include <qobject.h>
#include <qjsonobject.h>

class Receiver : public QObject {
	Q_OBJECT

public:
	explicit Receiver(QObject* parent = 0);
	void updateChatList(QJsonArray chats);

signals:
	void appendChat(QJsonObject chat);

public slots:
	void receiveFromQml();
};

#endif // !RECEIVER_H
