#include "messenger-window.h"
#include "ui_messenger-window.h"
#include "client.h"

MessengerWindow::MessengerWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MessengerWindow) {
	ui->setupUi(this);
}

MessengerWindow::~MessengerWindow() {
	delete ui;
}
