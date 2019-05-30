/*!
 * \file models/File.h
 * \author Nikita Mironov <nickfrom22nd@gmail.com>
 * \brief The file entity for wrapping API responses
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


#ifndef ANTARCTICA_FILE_H
#define ANTARCTICA_FILE_H


#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <utility>
#include <QtCore/QJsonObject>
#include <QtCore/QVariant>

#include "Package.h"

using namespace std;

class File {
public:
    unsigned id;
    QString name;
    QString path;
    QByteArray content;
    QDateTime created;
    QDateTime modified;
    Package *package;

    File(int id, QString name, QString path, QDateTime created, QDateTime modified, uint packageId)
            : id(id), name(move(name)), path(move(path)),
              created(move(created)), modified(move(modified)) {}

    File(int id, QString name, QString path, QByteArray content, QDateTime created, QDateTime modified, uint packageId)
            : id(id), name(move(name)), path(move(path)), content(move(content)),
              created(move(created)), modified(move(modified)) {}

    File(QJsonObject &fileJson) {
        id = fileJson["id"].toInt();
        name = fileJson["name"].toString();
        path = fileJson["path"].toString();
        content = QByteArray::fromBase64(fileJson["content"].toVariant().toByteArray());
        created = fileJson["created"].toVariant().toDateTime();
        modified = fileJson["modified"].toVariant().toDateTime();
        package = new Package(fileJson["package"].toObject());
    }
};


#endif //ANTARCTICA_FILE_H
