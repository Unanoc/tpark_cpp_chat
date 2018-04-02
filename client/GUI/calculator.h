//
// Created by comp on 4/2/18.
//

#ifndef CLIENT_CALCULATOR_H
#define CLIENT_CALCULATOR_H

#include <QWidget>

class QLineEdit;

class Button;

class Calculator : public QWidget {
Q_OBJECT

public:
    Calculator(QWidget *parent = 0);

private slots:

    void digitClicked();

    void unaryOperatorClicked();

    void additiveOperatorClicked();

    void multiplicativeOperatorClicked();

    void equalClicked();

    void pointClicked();

    void changeSignClicked();

    void backspaceClicked();

    void clear();

    void clearAll();

    void clearMemory();

    void readMemory();

    void setMemory();

    void addToMemory();

private:
    Button *createButton(const QString &text, const char *member);

    void abortOperation();

    bool calculate(double rightOperand, const QString &pendingOperator);

    double sumInMemory;
    double sumSoFar;
    double factorSoFar;
    QString pendingAdditiveOperator;
    QString pendingMultiplicativeOperator;
    bool waitingForOperand;

    QLineEdit *display;

    enum {
        NumDigitButtons = 10
    };
    Button *digitButtons[NumDigitButtons];
};

#endif //CLIENT_CALCULATOR_H
