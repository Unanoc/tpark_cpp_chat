#include <QNetworkAccessManager>
#include <QtNetwork>
#include <iostream>
#include <map>
#include <utility>

#include "client.h"


std::map<std::pair<std::string,std::string>,Client*> Client::instances;

Client *Client::GetInstance(const std::string &login, const std::string &password) {
    auto itClient = instances.find({login, password});
    if (itClient != instances.end())
        return (*itClient).second;
    Client *client = new Client(login, password);
    instances[{login, password}] = client;
    return client;
}

void Client::destroyAll() {
    for (auto it = instances.begin(); it != instances.end(); ++it)
        delete (*it).second;
    instances.clear();
}

Client::Client(const std::string &login, const std::string &password, QWidget *parent) {
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply * )), this, SLOT(replyFinished(QNetworkReply * )));
    this->login = login;
    this->password = password;
}

QNetworkRequest Client::createRequest(QUrl qUrl) {

    QNetworkRequest request;
    request.setUrl(qUrl);
    request.setRawHeader("Content-Type", "application/json");
    return request;
}


void Client::Print() {

    QNetworkRequest request = createRequest(QUrl(""));

    manager->get(request);
}

void Client::replyFinished(QNetworkReply *reply) {
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }

    QJsonObject answer = parseReply(reply);
    qDebug() << answer;
}

QJsonObject Client::parseReply(QNetworkReply *reply) {
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;
    QJsonParseError parseError;
    QByteArray replyText = reply->readAll();
    jsonDoc = QJsonDocument::fromJson(replyText, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Json parse error: " << parseError.errorString();
    } else {
        if (jsonDoc.isObject())
            jsonObj = jsonDoc.object();
        else if (jsonDoc.isArray())
            jsonObj["non_field_errors"] = jsonDoc.array();
    }
    return jsonObj;
}