#include "server.hpp"

namespace JETPP
{
    Server::Server(Router router)
    {
        this->router = router;
    }

    void Server::sendResponse(int clientSocket, const char *response)
    {
        send(clientSocket, response, strlen(response), 0);
    }

    void Server::start(int port)
    {
        int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == -1)
        {
            std::cerr << "Failed to create socket" << std::endl;
            return;
        }

        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        serverAddress.sin_port = htons(port);

        if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
        {
            std::cerr << "Bind failed" << std::endl;
            close(serverSocket);
            return;
        }

        if (listen(serverSocket, SOMAXCONN) == -1)
        {
            std::cerr << "Listen failed" << std::endl;
            close(serverSocket);
            return;
        }

        std::cout << "Server is listening on port " << port << std::endl;

        while (true)
        {
            int clientSocket = accept(serverSocket, NULL, NULL);
            if (clientSocket == -1)
            {
                std::cerr << "Accept failed" << std::endl;
                close(serverSocket);
                return;
            }

            // Create a thread for each client
            std::thread clientThread(&Server::handleClient, this, clientSocket);
            clientThread.detach();  // Detach the thread to allow it to run independently
        }

        close(serverSocket);
    }

    void Server::handleClient(int clientSocket)
    {
        char buffer[1024];
        std::string request;
        int totalBytesRead = 0;

        while (true)
            {
                int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
                if (bytesRead == -1)
                {
                    std::cerr << "Receive failed" << std::endl;
                    close(clientSocket);
                    break;
                }
                else if (bytesRead == 0)
                {
                    // Connection closed by the client
                    close(clientSocket);
                    break;
                }
                else
                {
                    request.append(buffer, bytesRead);
                    totalBytesRead += bytesRead;

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
                int methodEnd = request.find(' ');
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
                        Route route = this->router.findRoute(url, stringToMethod(method));
                        Request req(url, route.getName(), request);
                        Response res(clientSocket);
                        route.execute(req, res);
                    }
                    catch (const std::runtime_error &)
                    {
                        const char *response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
                        sendResponse(clientSocket, response);
                    }
                }
            }

        close(clientSocket); // close the client socket when done
    }
}