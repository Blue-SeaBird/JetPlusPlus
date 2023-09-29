#include "server.hpp"
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

namespace JETPP
{
    Server::Server(Router router)
    {
        this->router = router;
    }

    void Server::sendResponse(SOCKET clientSocket, const char *response)
    {
        send(clientSocket, response, strlen(response), 0);
    }

    void Server::start(int port)
    {
        this->port = port;
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            std::cerr << "Failed to initialize Winsock" << std::endl;
        }

        SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == INVALID_SOCKET)
        {
            std::cerr << "Failed to create socket" << std::endl;
            WSACleanup();
        }

        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
        serverAddress.sin_port = htons(port);

        if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
        {
            std::cerr << "Bind failed" << std::endl;
            closesocket(serverSocket);
            WSACleanup();
        }

        if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
        {
            std::cerr << "Listen failed" << std::endl;
            closesocket(serverSocket);
            WSACleanup();
        }

        std::cout << "Server is listening on port " << port << std::endl;

        while (true)
        {
            SOCKET clientSocket = accept(serverSocket, NULL, NULL);
            if (clientSocket == INVALID_SOCKET)
            {
                std::cerr << "Accept failed" << std::endl;
                closesocket(serverSocket);
                WSACleanup();
            }

            char buffer[1024];
            int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytesRead == SOCKET_ERROR)
            {
                std::cerr << "Receive failed" << std::endl;
                closesocket(clientSocket);
                continue;
            }

            // Process the HTTP request here

            std::string request(buffer, bytesRead);
            bool routeMatched = false;

            // Iterate through all registered routes
            for (int i = 0; i < this->router.getRoutes().size(); i++)
            {
                if (request.find(this->router.getRoutes()[i].getName()) != std::string::npos)
                {
                    // Matching route found, set the flag to true
                    routeMatched = true;
                    JETPP::Request req;
                    JETPP::Response res(clientSocket);
                    this->router.getRoutes()[i].execute(req, res);

                    // Construct and send the response for the matched route
                    std::string responseMessage = "Hello, Users! Route: " + this->router.getRoutes()[i].getName();
                    std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(responseMessage.length()) + "\r\n\r\n" + responseMessage;
                    // sendResponse(clientSocket, response.c_str());
                    break;
                }
            }

            // If no matching route was found, send a 404 response
            if (!routeMatched)
            {
                const char *response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
                sendResponse(clientSocket, response);
            }

            closesocket(clientSocket);
        }

        closesocket(serverSocket);
        WSACleanup();
    }
}
