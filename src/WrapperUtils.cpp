#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QJsonDocument>
#include <QtCore/QCoreApplication>
#include <QtCore/QUrlQuery>

#include "api/Wrapper.h"

QJsonDocument Wrapper::Utils::execute(const QUrl &requestUrl, RequestType type) {
    qDebug() << "Executing " + requestUrl.toString();

    auto manager = new QNetworkAccessManager;

    QNetworkRequest request(requestUrl);
    request.setSslConfiguration(sslConfiguration);
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
Wrapper::Utils::executeForm(const QUrl &requestUrl, QHttpMultiPart *formData, Wrapper::Utils::RequestType type) {
    qDebug() << "Executing " + requestUrl.toString();

    auto manager = new QNetworkAccessManager;

    QNetworkRequest request(requestUrl);
    request.setSslConfiguration(sslConfiguration);
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
Wrapper::Utils::executeForm(const QUrl &requestUrl, QUrlQuery &formData, Wrapper::Utils::RequestType type) {
    qDebug() << "Executing " + requestUrl.toString();

    auto manager = new QNetworkAccessManager;

    QNetworkRequest request(requestUrl);
    request.setSslConfiguration(sslConfiguration);
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

QHttpMultiPart *Wrapper::Utils::generateMultipart(const File *file) {
    auto multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart pathPart;
    pathPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(R"(form-data; name="path")"));
    pathPart.setBody(file->path.toUtf8());
    multiPart->append(pathPart);

    QHttpPart createdTimePart;
    createdTimePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(R"(form-data; name="created")"));
    createdTimePart.setBody(QByteArray::number(file->created.toSecsSinceEpoch()));
    multiPart->append(createdTimePart);

    QHttpPart modifiedTimePart;
    modifiedTimePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(R"(form-data; name="modified")"));
    modifiedTimePart.setBody(QByteArray::number(file->modified.toSecsSinceEpoch()));
    multiPart->append(modifiedTimePart);

    QHttpPart packageIDPart;
    packageIDPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(R"(form-data; name="package_id")"));
    packageIDPart.setBody(QByteArray::number(file->package->id));
    multiPart->append(packageIDPart);

    QHttpPart fileDataPart;
    fileDataPart.setHeader(
            QNetworkRequest::ContentDispositionHeader,
            QVariant(QString(R"(form-data; name="upload"; filename="%1")").arg(file->name))
    );
    fileDataPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/octet-stream"));
    fileDataPart.setBody(file->content);
    multiPart->append(fileDataPart);

    return multiPart;
}
