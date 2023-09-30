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
        void send(std::string message);
        template <typename T>
        void send(T &object);
        void sendFile(std::string path);
    };
}

#endif
