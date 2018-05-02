#include "receiver.h"
#include <qdebug.h>
#include <qjsonarray.h>

Receiver::Receiver(QObject* parent) : QObject(parent) {}

void Receiver::receiveFromQml() {
    emit appendChat({{"name", "test"},{"message", "Lorem Ipsum"},{"avatar","qrc:/Resources/images/icon_avatarmaleinv.png"}});
}
