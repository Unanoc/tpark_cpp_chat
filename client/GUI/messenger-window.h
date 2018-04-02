#ifndef CLIENT_MESSENGER_WINDOW_H
#define CLIENT_MESSENGER_WINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow* ui;
};

#endif //CLIENT_MESSENGER_WINDOW_H
