/*!
 * \file
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

#ifndef ANTARCTICA_WRAPPER_H
#define ANTARCTICA_WRAPPER_H


#include <QtCore/QString>
#include <QtCore/QJsonArray>
#include <QtNetwork/QHttpMultiPart>
#include <QtCore/QJsonDocument>
#include <QtNetwork/QSslConfiguration>

#include "api/models/User.h"
#include "api/models/File.h"
#include "api/models/Package.h"
#include "api/models/Repository.h"
#include "api/models/Response.hpp"

using namespace std;

/*!
 * \class APIWrapper
 * \brief Wrapper for server REST API
 *
 * Class APIWrapper is a wrapper for server REST API and contains 3 subclasses:
 * Files, Packages and Repositories for interaction with specified API sections.
*/
class Wrapper {
    inline static QString serverAddr = "https://antarctica-server.tk";

public:
    /*!
     * \brief Set authentication data to static storage
     * \param id
     * \param token
     * \param sslConfig
     */
    static void init(const QSslConfiguration &sslConfig, bool local) {
        sslConfiguration = sslConfig;
        if (local) {
            serverAddr = "http://127.0.0.1:3000";
        }
    }

    /*!
     * \brief Authorize the user
     * \param login username
     * \param password user's password
     * \return User entity created from login server response
     */
    static User authorize(const QString &login, const QString &password);

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
        static const QList<Entity *> getAll();

        /*!
        * \brief Wrapper for get all API methods (GET request to "files", "pkgs" or "repos") wrapped into hashmap
        * \tparam Entity Entity type: File, Package or Repository
        * \return Mapped found entities
        */
        static const QMap<QString, Entity *> getAllMapped();

        /*!
        * \brief Wrapper for get API methods (GET request to "file/{id}", "pkg/{id}" or "repo/{id}")
        * \tparam Entity
        * \param id ID of entity to search
        * \return Found entity
        */
        static Entity *get(int id);

        /*!
        * \brief Wrapper for upload API methods (POST request to "files", "pkgs" or "repos")
        * \tparam Entity Entity type: File, Package or Repository
        * \param entity An entity to upload
        * \return Request status: recently created entity id
        */
        static int upload(const Entity *entity);

        /*!
        * \brief Wrapper for update API methods (PUT request to "files", "pkgs" or "repos")
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
        static bool remove(int id);
    };

    /*!
    * \class APIWrapper::Files
    * \brief Wrapper for files API section
    */
    class Files : public Section<File> {
    public:
        /*!
         * \brief Wrapper for getting contents of a specific file
         * \param id File id
         * \return File contents
         */
        static QByteArray getContent(int id);
    };

    /*!
    * \class APIWrapper::Packages
    * \brief Wrapper for packages API section
    */
    class Packages : public Section<Package> {
    public:
        /*!
         * \brief Wrapper for package getConfigs method
         * \param id Package id
         * \return List of files marked as given package's configs
         */
        static QList<File *> getConfigs(int id);
    };


    /*!
    * \class APIWrapper::Repositories
    * \brief Wrapper for repositories API section
    */
    class Repositories : public Section<Repository> {
    };

private:
    inline static User user; /**< User needed for API accessing */
    inline static QSslConfiguration sslConfiguration = QSslConfiguration::defaultConfiguration(); /**< An SSL configuration to perform an encrypted connection */

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

        static bool checkResponse(const Response &resp) {
            if (!resp.ok) {
                qDebug() << "Error code " << static_cast<int>(resp.error.code) << ": " << resp.error.text;
                return false;
            }
            return true;
        }

        static QHttpMultiPart *generateMultipart(const File *file);
    };

};


#endif //ANTARCTICA_WRAPPER_H
