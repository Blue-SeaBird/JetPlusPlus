#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <string>
#include <winsock2.h>
#include "json/value.hpp"

namespace JETPP
{
    class Response
    {
    private:
        int statuscode;
        SOCKET clientSocket;
        std::vector<std::string> header;

    public:
        Response(SOCKET clientSocket);
        void send(std::string message);
        void sendFile(std::string path);
        JETPP::Response status(int status);
        void json(JETPP::JsonValue object);
        void addHeader(const std::string &key, const std::string &value);
    };
}

#endif
