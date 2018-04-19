#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include <QNetworkReply>
#include <string>

class Client : public QObject {
Q_OBJECT
public:

    static Client *GetInstance(const std::string& login,const std::string& password);
    static void destroyAll();

    void Print();

private slots:

    void replyFinished(QNetworkReply *reply);

private:
    Client(const std::string& login,const std::string& password,QWidget *parent = nullptr);

    Client(Client const &) = delete;

    Client &operator=(Client const &) = delete;

    QNetworkRequest createRequest(QUrl qUrl);

    QJsonObject parseReply(QNetworkReply *reply);

    QNetworkAccessManager *manager;
    std::string login;
    std::string password;
    static std::map<std::pair<std::string,std::string>,Client*> instances;
};

#endif //CLIENT_CLIENT_H
