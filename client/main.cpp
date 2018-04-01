#include "messenger-window.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MessengerWindow mw;
    mw.show();

    return app.exec();
}
