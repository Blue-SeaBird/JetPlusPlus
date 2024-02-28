#include "../include/server/server.hpp"

namespace JETPP
{
    Server::Server(Router router)
    {
        this->router = router;
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

        std::cout << "\033[0;34m";
        std::cout << "     ____.       __    __________.__                 __________.__                " << std::endl;
        std::cout << "    |    | _____/  |_  \\______   \\  |  __ __  ______ \\______   \\  |  __ __  ______" << std::endl;
        std::cout << "    |    |/ __ \\   __\\  |     ___/  | |  |  \\/  ___/  |     ___/  | |  |  \\/  ___/" << std::endl;
        std::cout << "/\\__|    \\  ___/|  |    |    |   |  |_|  |  /\\___ \\   |    |   |  |_|  |  /\\___ \\ " << std::endl;
        std::cout << "\\________|\\___  >__|    |____|   |____/____//____  >  |____|   |____/____//____  >" << std::endl;
        std::cout << "              \\/                                 \\/                            \\/ " << std::endl;
        std::cout << "\033[0m"; 


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
                    // connection closed by the client
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
                        std::string clientAddress=getFullClientAddress(clientSocket);
                        std::optional<Route> optRoute = this->router.findRoute(url, stringToMethod(method), clientAddress);
                        if(!optRoute.has_value()){
                            const char *response = "HTTP/1.1 401 Unauthorized\r\nContent-Length: 0\r\n\r\n";
                            sendResponse(clientSocket, response);
                        }
                        else{
                            Route route=optRoute.value();
                            Request req(url, route.getName(), request);
                            Response res(clientSocket);
                            route.execute(req, res);
                        }
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

    void Server::sendResponse(int clientSocket, const char *response)
    {
        send(clientSocket, response, strlen(response), 0);
    }

    std::string Server::getFullClientAddress(int clientSocket)
    {
        sockaddr_in clientAddress;
        socklen_t clientAddrLen = sizeof(clientAddress);

        // retrieve client's address information
        if (getpeername(clientSocket, (struct sockaddr*)&clientAddress, &clientAddrLen) == -1)
        {
            std::cerr << "Failed to get client address" << std::endl;
            close(clientSocket);
            return nullptr;
        }

        char clientIP[INET_ADDRSTRLEN];
        if (inet_ntop(AF_INET, &(clientAddress.sin_addr), clientIP, INET_ADDRSTRLEN) == NULL)
        {
            std::cerr << "Failed to convert client address to string" << std::endl;
            close(clientSocket);
            return nullptr;
        }
        std::string fullAddress=std::string(clientIP)+ ":" +std::to_string(clientAddress.sin_port);

        return fullAddress;
    }
}