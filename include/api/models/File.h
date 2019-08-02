/*!
 * \file
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
#include <QtCore/QDir>

#include "Package.h"
#include "Entity.h"

using namespace std;

/*!
 * \class File
 * \brief The file entity for wrapping API responses
 */
class File : public Entity {
public:
    int id{};
    QString name;
    QString path;
    QByteArray content;
    QByteArray checksum;
    QDateTime created;
    QDateTime modified;
    Package *package;

    /*!
     * \brief Constructor for wrapping existing packages into a C++ class
     * \param id File id
     * \param name File name including extension
     * \param path Path where file is located
     * \param created File creation time and date
     * \param modified Last modified time and date
     * \param content File content, by default empty
     * \param pkg Package which file is according to, by default predefined package with id 1
     */
    File(int id, QString name, QString path, QByteArray checksum, QDateTime created, QDateTime modified,
         QByteArray content = QByteArray(), const Package *pkg = Package::Default)
            : id(id), name(move(name)), path(move(path)), checksum(move(checksum)),
              created(move(created)), modified(move(modified)),
              content(move(content)), package(const_cast<Package *>(pkg)) {}

    /*!
     * \brief Constructor for generating new Packages for uploading into server
     * \param id File id
     * \param name File name including extension
     * \param path Path where file is located
     * \param created File creation time and date
     * \param modified Last modified time and date
     * \param content File content, by default empty
     * \param pkg Package which file is according to, by default predefined package with id 1
     */
    File(QString name, QString path, QByteArray checksum, QDateTime created, QDateTime modified,
         QByteArray content = QByteArray(), const Package *pkg = Package::Default)
            : name(move(name)), path(move(path)), checksum(move(checksum)),
              created(move(created)), modified(move(modified)),
              content(move(content)), package(const_cast<Package *>(pkg)) {}

    /*!
     * \brief Constructor for wrapping server JSON responses into a C++ class
     * \param fileJson A JSON response from server containing a file object
     */
    explicit File(QJsonObject &fileJson) {
        id = fileJson["id"].toInt();
        name = fileJson["name"].toString();
        path = fileJson["path"].toString();
        if (path.endsWith('/')) {
            path.remove(path.size() - 1, 1);
        }
        content = QByteArray::fromBase64(fileJson["content"].toVariant().toByteArray());
        checksum = fileJson["checksum"].toVariant().toByteArray();
        created = fileJson["created"].toVariant().toDateTime();
        modified = fileJson["modified"].toVariant().toDateTime();
        package = new Package(fileJson["package"].toObject());
    }

    inline const QString getAbsolutePath() const {
        return QString(path).replace("~", QDir::homePath());
    };

    inline const QString getAbsoluteName() const {
        return QString(path+"/"+name).replace("~", QDir::homePath());
    }

    inline const QString getRelativeName() const {
        return QString(path+"/"+name);
    }

    ~File() override = default;
};


#endif //ANTARCTICA_FILE_H
