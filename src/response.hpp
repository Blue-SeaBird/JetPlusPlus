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
        int status;
        SOCKET clientSocket;

    public:
        Response(SOCKET clientSocket);
        void send(int status, std::string message);
        void sendFile();
    };
}

#endif
