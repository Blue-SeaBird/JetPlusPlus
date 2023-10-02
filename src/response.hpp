#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <iostream>
#include <string>
#include <winsock2.h>

namespace JETPP
{
    class Response
    {
    private:
        int statuscode;
        SOCKET clientSocket;

    public:
        Response(SOCKET clientSocket);
        void send(std::string message);
        void sendFile(std::string path);
        void status(int status);
    };
}

#endif
