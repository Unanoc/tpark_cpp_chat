#include "receiver.h"
#include <qdebug.h>
#include <qjsonarray.h>

Receiver::Receiver(QObject* parent) : QObject(parent) {}

void Receiver::updateChatList(QJsonArray chats) {

}

void Receiver::receiveFromQml() {
	qDebug() << "Received in C++ from QML";
}