#include <qguiapplication.h>
#include <qqmlapplicationengine.h>
#include <qqmlcontext.h>
#include "GUI/receiver.h"

int main(int argc, char* argv[]) {
	QGuiApplication app(argc, argv);
	QCoreApplication::addLibraryPath("./");
	QQmlApplicationEngine engine;

	Receiver receiver;
	QQmlContext* ctx = engine.rootContext();
	ctx->setContextProperty("receiver", &receiver);

	engine.load(QUrl(QStringLiteral("qrc:/MessengerWindow.qml")));
	return app.exec();
}