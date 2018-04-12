#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include <QNetworkReply>

class Client: public QObject{
Q_OBJECT
public:
    Client(QWidget *parent = nullptr);
    void Print();
private slots:
    void replyFinished(QNetworkReply *reply);
private:
    QNetworkRequest createRequest();
    QJsonObject parseReply(QNetworkReply *reply);

    QNetworkAccessManager *manager;


};

#endif //CLIENT_CLIENT_H
