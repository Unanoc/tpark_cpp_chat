#include "receiver.h"
#include <qdebug.h>
#include <qjsonobject.h>
#include <qvariant.h>

Receiver::Receiver(QObject* parent) : QObject(parent) {}

void Receiver::chatSlot() {
    emit appendChat({{"name", "test"},{"message", "Lorem Ipsum"},{"avatar","qrc:/Resources/images/icon_avatarmaleinv.png"}});
}

void Receiver::getSlot(const QVariant& var) {
	QJsonObject json = var.toJsonObject();
	qDebug() << json;
	qDebug() << var;
}
