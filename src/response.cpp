#include "response.hpp"
#include <string>
#include <stdio.h>
#include <iostream>

namespace JETPP
{
    Response::Response(SOCKET clientSocket)
    {
        this->clientSocket = clientSocket;
    }

    void Response::send(int status, std::string message)
    {
        this->status = status;
        std::cout << this->status << " " << message << std::endl;
        std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(message.length()) + "\r\n\r\n" + message;
        _WINSOCK2_H::send(clientSocket, response.c_str(), strlen(response.c_str()), 0);
    }

}