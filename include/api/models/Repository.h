/*!
 * \file
 * \author Nikita Mironov <nickfrom22nd@gmail.com>
 * \brief The repository entity for wrapping API responses
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

#ifndef ANTARCTICA_REPOSITORY_H
#define ANTARCTICA_REPOSITORY_H


#include <QtCore/QString>
#include <QtCore/QJsonObject>

#include "Entity.h"

using namespace std;

/*!
 * \class Repository
 * \brief The repository entity for wrapping API responses
 */
class Repository : public Entity {
    static Repository _noRepo;
    static Repository _default;

public:
    int id{};
    QString name;
    QString url;
    QString manager;

    /*!
     * \brief Constructor for wrapping existing packages into a C++ class
     * \param id Repository id
     * \param name Repository name
     * \param url Repository base URL
     * \param manager Package manager name
     */
    explicit Repository(int id, QString name, QString url, QString manager)
            : id(id), name(move(name)), url(move(url)), manager(move(manager)) {}

    /*!
     * \brief Constructor for wrapping existing packages into a C++ class
     * \param name Repository name
     * \param url Repository base URL
     * \param manager Package manager name
     */
    explicit Repository(QString name, QString url, QString manager)
            : name(move(name)), url(move(url)), manager(move(manager)) {}

    /*!
     * \brief Constructor for wrapping server JSON responses into a C++ class
     * \param repoJson A JSON response from server containing a repository object
     */
    explicit Repository(QJsonObject repoJson) {
        id = repoJson["id"].toInt();
        name = repoJson["name"].toString();
        url = repoJson["url"].toString();
        manager = repoJson["manager"].toString();
    }

    /*!
     * \brief Predefined no repository (id 1) entity for user files
     */
    inline static constexpr Repository *NoRepo = &Repository::_noRepo;
    /*!
     * \brief Predefined default repository (id 2) entity for packages which not according to a specific repository
     */
    inline static constexpr Repository *Default = &Repository::_default;
};


#endif //ANTARCTICA_REPOSITORY_H
