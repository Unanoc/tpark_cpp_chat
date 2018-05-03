#pragma once
#ifndef RECEIVER_H
#define RECEIVER_H

#include <qobject.h>
#include <qjsonobject.h>
#include <qvariant.h>

class Receiver : public QObject {
	Q_OBJECT

public:
	explicit Receiver(QObject* parent = 0);

signals:
	void appendChat(QJsonObject chat);

public slots:
    void chatSlot();
    void getSlot(const QVariant& var);
};

#endif // !RECEIVER_H
