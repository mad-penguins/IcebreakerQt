/*!
 * \file utils/api/APIWrapper.cpp
 * \author Nikita Mironov <nickfrom22nd@gmail.com>
 * \brief The API wrapper implementation
 *
 * \section LICENSE
 *
 * Copyright (c) 2019 Penguins of Madagascar

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#include <QtCore/QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QJsonDocument>
#include <QtCore/QCoreApplication>
#include <QtCore/QUrlQuery>
#include <QtCore/QMimeDatabase>

#include "Wrapper.h"
#include "models/Response.hpp"

Package Package::_default = Package(1, "");
Repository Repository::_noRepo = Repository(1, "", "", "");
Repository Repository::_default = Repository(2, "Default", "", "");

User Wrapper::user = User();
QString Wrapper::serverAddr = "https://antarctica-server.tk";
QSslConfiguration Wrapper::sslConfiguration = QSslConfiguration::defaultConfiguration();

User Wrapper::authorize(const QString &login, const QString &password) {
    auto loginUrl = QUrl(Wrapper::serverAddr + "/api/login");

    QUrlQuery formData;
    formData.addQueryItem("login", login);
    formData.addQueryItem("password", password);

    auto json = Utils::executeForm(loginUrl, formData, Utils::POST);
    auto respJson = json.object();

    auto resp = Response(respJson);
    if (!Utils::checkResponse(resp)) {
        throw Response::Exception(resp.error.code);
    }

    auto userJson = respJson["user"].toObject();
    if (!userJson.keys().contains("id")
        && !userJson.keys().contains("login")
        && !userJson.keys().contains("name")
        && !userJson.keys().contains("token")) {
        throw Response::Exception(Response::Error::MissingFields);
    }
    auto usr = User(userJson);
    Wrapper::user = usr;
    return usr;
}

// init specified static fields
template<> QString Wrapper::Section<File>::prefix = "file";
template<> QString Wrapper::Section<Package>::prefix = "pkg";
template<> QString Wrapper::Section<Repository>::prefix = "repo";

template<class Entity>
QList<Entity *> Wrapper::Section<Entity>::getAll() {
    auto getUrl = QUrl(
            QString(Wrapper::serverAddr + "/api/user/%1/%2s/%3").arg(
                    QString::number(user.id),
                    prefix,
                    user.accessToken)
    );
    auto json = Utils::execute(getUrl, Utils::GET);

    QList<Entity *> objects;
    if (Utils::checkResponse(Response(json.object()))) {
        auto respJson = json[prefix + "s"].toArray();
        for (auto &&val : respJson) {
            if (val.isObject()) {
                auto fileJson = val.toObject();
                objects << new Entity(fileJson);
            }
        }
    }
    return objects;
}

template<class Entity>
Entity *Wrapper::Section<Entity>::get(unsigned id) {
    auto getUrl = QUrl(
            QString(Wrapper::serverAddr + "/api/user/%1/%2/%3/%4").arg(
                    QString::number(user.id),
                    prefix,
                    QString::number(id),
                    user.accessToken
            )
    );
    auto json = Utils::execute(getUrl, Utils::GET);
    if (!Utils::checkResponse(Response(json.object()))) {
        return nullptr;
    }
    auto respJson = json[prefix].toObject();
    return new Entity(respJson);

}

template<>
int Wrapper::Section<File>::upload(const File *file) {
    auto uploadUrl = QUrl(
            QString(Wrapper::serverAddr + "/api/user/%1/%2s/%3").arg(
                    QString::number(user.id),
                    prefix,
                    user.accessToken
            )
    );
    auto json = Utils::executeForm(uploadUrl, Utils::generateMultipart(file), Utils::POST);
    if (Utils::checkResponse(Response(json.object()))) {
        return json.object()["created_id"].toInt();
    } else {
        return -1;
    }
}

template<>
int Wrapper::Section<Package>::upload(const Package *pkg) {
    auto updateUrl = QUrl(
            QString(Wrapper::serverAddr + "/api/user/%1/%2s/%3").arg(
                    QString::number(user.id),
                    prefix,
                    user.accessToken
            )
    );

    QUrlQuery formData;
    formData.addQueryItem("id", QString::number(pkg->id));
    formData.addQueryItem("name", pkg->name);
    formData.addQueryItem("repo_id", QString::number(pkg->repository->id));

    auto json = Utils::executeForm(updateUrl, formData, Utils::POST);
    if (Utils::checkResponse(Response(json.object()))) {
        return json.object()["created_id"].toInt();
    } else {
        return -1;
    }
}

template<>
int Wrapper::Section<Repository>::upload(const Repository *repo) {
    auto updateUrl = QUrl(
            QString(Wrapper::serverAddr + "/api/user/%1/%2s/%3").arg(
                    QString::number(user.id),
                    prefix,
                    user.accessToken
            )
    );

    QUrlQuery formData;
    formData.addQueryItem("id", QString::number(repo->id));
    formData.addQueryItem("name", repo->name);
    formData.addQueryItem("url", repo->url);
    formData.addQueryItem("manager", repo->manager);

    auto json = Utils::executeForm(updateUrl, formData, Utils::POST);
    if (Utils::checkResponse(Response(json.object()))) {
        return json.object()["created_id"].toInt();
    } else {
        return -1;
    }
}

template<>
bool Wrapper::Section<File>::update(const File *file) {
    auto uploadUrl = QUrl(
            QString(Wrapper::serverAddr + "/api/user/%1/%2s/%3").arg(
                    QString::number(user.id),
                    prefix,
                    user.accessToken
            )
    );
    auto json = Utils::executeForm(uploadUrl, Utils::generateMultipart(file), Utils::PUT);
    return Utils::checkResponse(Response(json.object()));
}

template<>
bool Wrapper::Section<Package>::update(const Package *pkg) {
    auto updateUrl = QUrl(
            QString(Wrapper::serverAddr + "/api/user/%1/%2s/%3").arg(
                    QString::number(user.id),
                    prefix,
                    user.accessToken
            )
    );

    QUrlQuery formData;
    formData.addQueryItem("name", pkg->name);
    formData.addQueryItem("repo_id", QString::number(pkg->repository->id));

    auto json = Utils::executeForm(updateUrl, formData, Utils::PUT);
    return Utils::checkResponse(Response(json.object()));
}

template<>
bool Wrapper::Section<Repository>::update(const Repository *repo) {
    auto updateUrl = QUrl(
            QString(Wrapper::serverAddr + "/api/user/%1/%2s/%3").arg(
                    QString::number(user.id),
                    prefix,
                    user.accessToken
            )
    );

    QUrlQuery formData;
    formData.addQueryItem("name", repo->name);
    formData.addQueryItem("url", repo->url);
    formData.addQueryItem("manager", repo->manager);

    auto json = Utils::executeForm(updateUrl, formData, Utils::PUT);
    return Utils::checkResponse(Response(json.object()));
}

template<class Entity>
bool Wrapper::Section<Entity>::remove(unsigned id) {
    auto deleteFileUrl = QUrl(
            QString(Wrapper::serverAddr + "/api/user/%1/%2/%3/%4").arg(
                    QString::number(user.id),
                    prefix,
                    QString::number(id),
                    user.accessToken
            )
    );
    auto json = Utils::execute(deleteFileUrl, Utils::DELETE);
    return Utils::checkResponse(Response(json.object()));
}

QList<File *> Wrapper::Packages::getConfigs(unsigned id) {
    auto getConfigsUrl = QUrl(
            QString(Wrapper::serverAddr + "/api/user/%1/pkg/%2/configs/%3").arg(
                    QString::number(user.id),
                    QString::number(id),
                    user.accessToken
            )
    );
    auto json = Utils::execute(getConfigsUrl, Utils::GET);

    QList<File *> configs;
    if (Utils::checkResponse(Response(json.object()))) {
        auto respJson = json["files"].toArray();
        for (auto &&val : respJson) {
            if (val.isObject()) {
                auto fileJson = val.toObject();
                configs << new File(fileJson);
            }
        }
    }
    return configs;
}

// tell the compiler to "implement" methods from super class
template
class Wrapper::Section<File>;

template
class Wrapper::Section<Package>;

template
class Wrapper::Section<Repository>;
