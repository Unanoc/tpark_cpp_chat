//
// Created by comp on 4/2/18.
//

#ifndef CLIENT_BUTTON_H
#define CLIENT_BUTTON_H

#include <QToolButton>

class Button : public QToolButton {
    Q_OBJECT

public:
    explicit Button(const QString &text, QWidget *parent = 0);

    QSize sizeHint() const override;
};

#endif //CLIENT_BUTTON_H
