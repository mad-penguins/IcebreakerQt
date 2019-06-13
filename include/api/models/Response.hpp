/*!
 * \file
 * \author Nikita Mironov <nickfrom22nd@gmail.com>
 * \brief The response entity for wrapping and checking API responses
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

#ifndef ANTARCTICA_RESPONSE_HPP
#define ANTARCTICA_RESPONSE_HPP

#include <QtCore/QString>
#include <QtCore/QJsonObject>

/*!
 * \class Resposnse
 * \brief The response entity for wrapping and checking API responses
 */
class Response {
public:
    bool ok;

    /*!
     * \class Reponse::Error
     * \brief Class describing an error in reponse
     */
    class Error {
    public:
        /*!
         * \enum Code
         * \brief Enum containing available error codes
         */
        enum class Code {
            NotFound = 1, ///< Record not found
            InvalidToken = 2, ///< Invalid or expired access token
            FormParsingError = 3, ///< An error occurred while form parsing
            AlreadyExists = 4, ///< An object already exists at the server
            WrongLogin = 5, ///< Wrong login data (username or password)
            OK = 0, ///< OK
            NoResponse = -1, ///< Got no response from server
            MissingFields = -2 ///< Got a response, but some fields are mising
        } code = Code::NoResponse;
        QString text = "got no response";

        Error() = default;

        explicit Error(Code code) : code(code) {};

        /*!
        * \brief Constructor for wrapping server JSON responses into a C++ class
        * \param errJson A JSON response from server containing am error object
        */
        explicit Error(QJsonObject errJson) {
            code = static_cast<Code>(errJson["code"].toInt());
            text = errJson["text"].toString();
        }
    } error;

    /*!
     * \class Response::Exception
     * \brief Class which describes an exception to throw in cause of response containig an error
     */
    class Exception : public exception {
    public:
        /*!
         * \brief Error code
         */
        Error::Code code;

        explicit Exception(Error::Code code) : code(code) {}
    };

    /*!
     * \brief Constructor for wrapping server JSON responses into a C++ class
     * \param respJson A JSON response from server
     */
    explicit Response(QJsonObject respJson) {
        ok = respJson["ok"].toBool();
        if (respJson.contains("error") && respJson["error"].isObject()) {
            error = Error(respJson["error"].toObject());
        } else if (
                !((respJson.contains("files") && respJson["files"].isArray())
                  || (respJson.contains("file") && respJson["file"].isObject())
                  || (respJson.contains("pkgs") && respJson["pkgs"].isArray())
                  || (respJson.contains("pkg") && respJson["pkg"].isObject())
                  || (respJson.contains("repos") && respJson["repos"].isArray())
                  || (respJson.contains("repo") && respJson["repo"].isObject())
                  || (respJson.contains("user") && respJson["user"].isObject())
                  || (respJson.contains("created_id")))) {
            ok = false;
            error.code = Error::Code::MissingFields;
        }
        if (ok) {
            error.code = Error::Code::OK;
        }
    }
};

#endif //ANTARCTICA_RESPONSE_HPP
