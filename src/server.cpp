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
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            std::cerr << "Failed to initialize Winsock" << std::endl;
            return;
        }

        SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == INVALID_SOCKET)
        {
            std::cerr << "Failed to create socket" << std::endl;
            WSACleanup();
            return;
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
            return;
        }

        if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
        {
            std::cerr << "Listen failed" << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return;
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
                return;
            }

            char buffer[1024];
            std::string request;    // Store the full request
            int totalBytesRead = 0; // Total bytes read so far

            while (true)
            {
                int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
                if (bytesRead == SOCKET_ERROR)
                {
                    std::cerr << "Receive failed" << std::endl;
                    closesocket(clientSocket);
                    break;
                }
                else if (bytesRead == 0)
                {
                    // Connection closed by the client
                    closesocket(clientSocket);
                    break;
                }
                else
                {
                    // Append the received data to the request
                    request.append(buffer, bytesRead);
                    totalBytesRead += bytesRead;

                    // Check if received the full request (end with double CRLF)
                    size_t bodyStart = request.find("\r\n\r\n");
                    if (bodyStart != std::string::npos)
                    {
                        size_t contentLengthPos = request.find("Content-Length: ");
                        if (contentLengthPos != std::string::npos)
                        {
                            size_t bodyLength = std::stoi(request.substr(contentLengthPos + 16, bodyStart - contentLengthPos - 16));
                            if (bodyStart + 4 + bodyLength <= totalBytesRead)
                            {
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }

            if (!request.empty())
            {
                int methodEnd = request.find(' '); // Find the first space
                if (methodEnd != std::string::npos)
                {
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
                        Request req(url, route.getName(), request);
                        Response res(clientSocket);
                        route.execute(req, res);
                    }
                    catch (const std::runtime_error &)
                    {
                        // Route not found
                        const char *response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
                        sendResponse(clientSocket, response);
                    }
                }
            }

            // Close the client socket after processing the request
            closesocket(clientSocket);
        }

        closesocket(serverSocket);
        WSACleanup();
    }
}
