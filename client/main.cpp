<<<<<<< HEAD
#include <QApplication>

#include "calculator.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Calculator calc;
    calc.show();
    return app.exec();
}
=======
#include "messenger-window.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MainWindow mw;
    mw.show();

    return app.exec();
}
>>>>>>> GUI
