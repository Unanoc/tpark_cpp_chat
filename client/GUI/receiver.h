#pragma once
#ifndef RECEIVER_H
#define RECEIVER_H

#include <qobject.h>

class Receiver : public QObject {
	Q_OBJECT

public:
	explicit Receiver(QObject* parent = 0);

signals:
	void sendToQml();

public slots:
	void receiveFromQml();
};

#endif // !RECEIVER_H
