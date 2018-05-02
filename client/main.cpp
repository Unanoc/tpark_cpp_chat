#include <qguiapplication.h>
#include <qqmlapplicationengine.h>
#include <qqmlcontext.h>
#include <qdebug.h>
#include <qvariant.h>
#include "GUI/receiver.h"

int main(int argc, char* argv[]) {
	QGuiApplication app(argc, argv);
	QCoreApplication::addLibraryPath("./");
	QQmlApplicationEngine engine;

	Receiver receiver;
	QQmlContext* ctx = engine.rootContext();
	ctx->setContextProperty("receiver", &receiver);

	engine.load(QUrl(QStringLiteral("qrc:/MessengerWindow.qml")));

    QObject* item = engine.rootObjects().first()->findChild<QObject*>("mainPage");

    QObject::connect(item, SIGNAL(dropInfo(QVariant)), &receiver, SLOT(getSlot(const QVariant&)));

	return app.exec();
}
