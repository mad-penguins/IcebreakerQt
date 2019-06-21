/*!
 * \file
 * \author Nikita Mironov <nickfrom22nd@gmail.com>
 * \brief The package entity for wrapping API responses
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

#ifndef ANTARCTICA_PACKAGE_H
#define ANTARCTICA_PACKAGE_H


#include <QtCore/QString>
#include "Repository.h"

#include "Entity.h"

using namespace std;

/*!
 * \class Package
 * \brief The package entity for wrapping API responses
 */
class Package : public Entity {
    static Package _default;
public:
    int id{};
    QString name;
    Repository *repository{};

    /*!
     * \brief Constructor for wrapping existing packages into a C++ class
     * \param id Package id
     * \param name Package name
     * \param repo Repository where the package according to, by default package has no repository
     */
    explicit Package(int id, QString name, const Repository *repo = Repository::NoRepo)
            : id(id), name(move(name)), repository(const_cast<Repository *>(repo)) {}

    /*!
     * \brief Constructor for generating new Packages for uploading into server
     * \param name Package name
     * \param repo Repository where the package according to, by default package has no repository
     */
    explicit Package(QString name, const Repository *repo = Repository::NoRepo)
            : name(move(name)), repository(const_cast<Repository *>(repo)) {}

    /*!
     * \brief Constructor for wrapping server JSON responses into a C++ class
     * \param pkgJson A JSON response from server containing a package object
     */
    explicit Package(QJsonObject pkgJson) {
        id = pkgJson["id"].toInt();
        name = pkgJson["name"].toString();
        repository = new Repository(pkgJson["repository"].toObject());
    }

    ~Package() override = default;

    /*!
     * \brief Predefined default package (id 1) entity for user files
     */
    inline static constexpr Package *Default = &_default;
};


#endif //ANTARCTICA_PACKAGE_H
