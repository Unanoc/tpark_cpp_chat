#include "receiver.h"
#include <qdebug.h>

Receiver::Receiver(QObject* parent) : QObject(parent) {}

void Receiver::receiveFromQml() {
	qDebug() << "Received in C++ from QML";
}