#include <QNetworkAccessManager>
#include <QtNetwork>
#include <iostream>

#include "client.h"


Client::Client(QWidget *parent){
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));
}

QNetworkRequest Client::createRequest() {

    QNetworkRequest request;
    request.setUrl(QUrl("https://query.yahooapis.com/v1/public/yql?q=select%20*%20from%20weather.forecast%20where%20woeid%20in%20(select%20woeid%20from%20geo.places(1)%20where%20text%3D%22nome%2C%20ak%22)&format=json&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys"));
    request.setRawHeader("Content-Type","application/json");
    return request;
}


void Client::Print() {

    QNetworkRequest request = createRequest();

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
    if(parseError.error != QJsonParseError::NoError){
        qDebug() << replyText;
        qWarning() << "Json parse error: " << parseError.errorString();
    }else{
        if(jsonDoc.isObject())
            jsonObj  = jsonDoc.object();
        else if (jsonDoc.isArray())
            jsonObj["non_field_errors"] = jsonDoc.array();
    }
    return jsonObj;
}