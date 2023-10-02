#include "server.hpp"

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

            // Process the HTTP request

            std::string request(buffer, bytesRead);

            // Get the method of the request
            int methodEnd = 0;
            for (int i = 0; i < request.size(); i++)
            {
                if (request[i] == ' ')
                {
                    methodEnd = i;
                    break;
                }
            }
            std::string method = request.substr(0, methodEnd);

            std::string url;
            for (int i = methodEnd + 1; i < request.size(); i++)
            {
                if (request.at(i) == ' ')
                {
                    url = request.substr(methodEnd + 1, i - methodEnd - 1);
                    break;
                }
            }

            try
            {
                // Route found
                Route route = this->router.findRoute(url, stringToMethod(method));
                Request req(url, route.getName());
                Response res(clientSocket);
                route.execute(req, res);
            }
            catch (const std::runtime_error &)
            {
                // Route not found
                const char *response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
                sendResponse(clientSocket, response);
            }

            closesocket(clientSocket);
        }

        closesocket(serverSocket);
        WSACleanup();
    }
}
