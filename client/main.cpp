#include <QGuiApplication>
#include <qqmlapplicationengine.h>

int main(int argc, char* argv[]) {
	QGuiApplication app(argc, argv);
	QCoreApplication::addLibraryPath("./");
	QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/MessengerWindow.qml")));

    return app.exec();
}
