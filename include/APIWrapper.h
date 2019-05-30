/*!
 * \file utils/api/APIWrapper.h
 * \author Nikita Mironov <nickfrom22nd@gmail.com>
 * \brief The API wrapper class
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

#ifndef ANTARCTICA_APIWRAPPER_H
#define ANTARCTICA_APIWRAPPER_H


#include <QtCore/QString>
#include <QtCore/QJsonArray>
#include <QtNetwork/QHttpMultiPart>
#include <QtCore/QJsonDocument>

#include "models/File.h"
#include "models/Package.h"
#include "models/Repository.h"
#include "models/Response.hpp"

using namespace std;

/*!
 * \class APIWrapper
 * \brief Wrapper for server REST API
 *
 * Class APIWrapper is a wrapper for server REST API and contains 3 subclasses:
 * Files, Packages and Repositories for interaction with specified API sections.
*/
class APIWrapper {
public:
    /*!
     * \brief Set authentication data to static storage
     * \param id
     * \param token
     */
    static void setUserData(unsigned id, QString token) {
        userId = id;
        accessToken = move(token);
    }

    /*!
     * \class APIWrapper::Section
     * \brief An abstraction to implement wrapper for API section
     * \tparam Entity Entity type: File, Package or Repository
     */
    template<class Entity>
    class Section {
    protected:
        static QString prefix;
    public:
        /*!
        * \brief Wrapper for get all API methods (GET request to "files", "pkgs" or "repos")
        * \tparam Entity Entity type: File, Package or Repository
        * \return List of found entities
        */
        static QList<Entity *> getAll();

        /*!
        * \brief Wrapper for get API methods (GET request to "file/{id}", "pkg/{id}" or "repo/{id}")
        * \tparam Entity
        * \param id ID of entity to search
        * \return Found entity
        */
        static Entity *get(unsigned id);

        /*!
        * \brief Wrapper for upload API methods (POST request to "files", "pkgs" or "repos")
        * \tparam Entity Entity type: File, Package or Repository
        * \param entity An entity to upload
        * \return Request status: ok or failed
        */
        static bool upload(const Entity *entity);

        /*!
        * \brief Wrapper for udate API methods (PUT request to "files", "pkgs" or "repos")
        * \tparam Entity Entity type: File, Package or Repository
        * \param entity An entity to update
        * \return Request status: ok or failed
        */
        static bool update(const Entity *entity);

        /*!
        * \brief Wrapper for delete API methods (DELETE request to "file/{id}", "pkg/{id}" or "repo/{id}")
        * \tparam Entity
        * \param id ID of entity to delete
        * \return Request status ok or failed
        */
        static bool remove(unsigned id);
    };

    /*!
    * \class APIWrapper::Files
    * \brief Wrapper for files API section
    */
    class Files : public Section<File> {
    };

    /*!
    * \class APIWrapper::Packages
    * \brief Wrapper for packages API section
    */
    class Packages : public Section<Package> {
    };


    /*!
    * \class APIWrapper::Repositories
    * \brief Wrapper for repositories API section
    */
    class Repositories : public Section<Repository> {
    };

private:
    static unsigned userId; /**< User id needed for API accessing */
    static QString accessToken; /**< Session access token needed for API authentication */

    /*!
     * \class APIWrapper::Utils
     * \brief Class with some some utilities for accessing REST API
     */
    class Utils {
    public:
        /*!
         * \brief HTTP Request type enum
         * RequestType enum contains HTTP request types needed for calling of methods
         * APIWrapper::Utils::execute(const QUrl &, RequestType)
         * and APIWrapper::Utils::executeForm(const QUrl &, QHttpMultipart *, RequestType)
         */
        enum RequestType {
            GET, POST, PUT, DELETE
        };

        /*!
         * \brief Execute an API request without form via GET or DELETE HTTP requests
         * \param requestUrl Prepared API request URL
         * \param type Type of HTTP request: GET or DELETE
         * \return JSON response
         */
        static QJsonDocument execute(const QUrl &requestUrl, RequestType type);

        /*!
         * \brief Execute an API request with a form via POST or PUT HTTP requests
         * \param requestUrl Prepared API request URL
         * \param formData Multipart form data
         * \param type Type of HTTP request: POST or PUT
         * \return JSON response
         */
        static QJsonDocument executeForm(const QUrl &requestUrl, QHttpMultiPart *formData, RequestType type);

        static QJsonDocument executeForm(const QUrl &requestUrl, QUrlQuery &formData, RequestType type);

        static bool checkResponse(Response resp) {
            if (!resp.ok) {
                qDebug() << "Error code " << resp.error.code << ": " << resp.error.text;
                return false;
            }
            return true;
        }
    };

};


#endif //ANTARCTICA_APIWRAPPER_H
