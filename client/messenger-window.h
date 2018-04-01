#ifndef CLIENT_MESSENGER_WINDOW_H
#define CLIENT_MESSENGER_WINDOW_H

#include <QMainWindow>

namespace Ui {
	class MessengerWindow;
}

class MessengerWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit MessengerWindow(QWidget* parent = 0);
	~MessengerWindow();

private:
	Ui::MessengerWindow* ui;
};

#endif //CLIENT_MESSENGER_WINDOW_H
