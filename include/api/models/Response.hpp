#ifndef ANTARCTICA_RESPONSE_HPP
#define ANTARCTICA_RESPONSE_HPP

#include <QtCore/QString>
#include <QtCore/QJsonObject>

class Response {
public:
    bool ok;

    class Error {
    public:
        enum Code {
            NotFound = 1, InvalidToken = 2, FormParsingError = 3, AlreadyExists = 4,
            OK = 0, NoResponse = -1, MissingFields = -2
        } code = Code::NoResponse;
        QString text = "got no response";

        Error() = default;

        explicit Error(Code code) : code(code) {};

        explicit Error(QJsonObject errJson) {
            code = static_cast<Code>(errJson["code"].toInt());
            text = errJson["text"].toString();
        }
    } error;

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
                  || (respJson.contains("repo") && respJson["repo"].isObject()))) {
            ok = false;
            error = Error(Error::Code::MissingFields);
        }
    }
};

#endif //ANTARCTICA_RESPONSE_HPP
