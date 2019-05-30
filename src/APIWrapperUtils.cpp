#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QJsonDocument>
#include <QtCore/QCoreApplication>
#include <QtCore/QUrlQuery>

#include "../include/APIWrapper.h"

QJsonDocument APIWrapper::Utils::execute(const QUrl &requestUrl, RequestType type) {
    qDebug() << "Executing " + requestUrl.toString();

    auto manager = new QNetworkAccessManager;

    QNetworkRequest request(requestUrl);
    QNetworkReply *reply;
    switch (type) {
        case GET:
            reply = manager->get(request);
            break;
        case DELETE:
            reply = manager->deleteResource(request);
            break;
        default:
            return QJsonDocument();
    }
    while (!reply->isFinished()) { // make thread not blocked by waiting for response
        qApp->processEvents();
    }

    QByteArray buffer = reply->readAll();
    auto json = QJsonDocument::fromJson(buffer);
    reply->deleteLater();
    return json;
}

QJsonDocument
APIWrapper::Utils::executeForm(const QUrl &requestUrl, QHttpMultiPart *formData, APIWrapper::Utils::RequestType type) {
    qDebug() << "Executing " + requestUrl.toString();

    auto manager = new QNetworkAccessManager;

    QNetworkRequest request(requestUrl);
    QNetworkReply *reply;
    switch (type) {
        case POST:
            reply = manager->post(request, formData);
            break;
        case PUT:
            reply = manager->put(request, formData);
            break;
        default:
            return QJsonDocument();
    }
    while (!reply->isFinished()) { // make thread not blocked by waiting for response
        qApp->processEvents();
    }

    QByteArray buffer = reply->readAll();
    qDebug() << buffer;
    auto json = QJsonDocument::fromJson(buffer);
    reply->deleteLater();
    return json;
}

QJsonDocument
APIWrapper::Utils::executeForm(const QUrl &requestUrl, QUrlQuery &formData, APIWrapper::Utils::RequestType type) {
    qDebug() << "Executing " + requestUrl.toString();

    auto manager = new QNetworkAccessManager;

    QNetworkRequest request(requestUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply *reply;
    switch (type) {
        case POST:
            reply = manager->post(request, formData.toString(QUrl::FullyEncoded).toUtf8());
            break;
        case PUT:
            reply = manager->put(request, formData.toString(QUrl::FullyEncoded).toUtf8());
            break;
        default:
            return QJsonDocument();
    }
    while (!reply->isFinished()) { // make thread not blocked by waiting for response
        qApp->processEvents();
    }

    QByteArray buffer = reply->readAll();
    qDebug() << buffer;
    auto json = QJsonDocument::fromJson(buffer);
    reply->deleteLater();
    return json;
}
