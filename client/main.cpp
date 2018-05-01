#include <QGuiApplication>
#include <qqmlapplicationengine.h>
#include <QObject>
#include <QtDebug>
#include <QQuickView>
#include <QQuickItem>

class Test : public QObject {
    Q_OBJECT
public slots:
    void cppSlot(const QVariant& v) {
        qInfo() << "OK";
    }
};

#include <main.moc>

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    QCoreApplication::addLibraryPath("./");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/MessengerWindow.qml")));

    //engine.rootObjects();
    //QQuickView view(QUrl(QStringLiteral("qrc:/MessengerWindow.qml")));
    //QObject* item = view.;

    /*
    for (int i = engine.rootObjects().begin; i < engine.rootObjects().end(); i++) {
        qInfo() << i;
    }
    */

    Test test;
    QObject::connect(item, SIGNAL(qmlSignal(QVariant)), &test, SLOT(cppSlot(QVariant)));



    return app.exec();
}
