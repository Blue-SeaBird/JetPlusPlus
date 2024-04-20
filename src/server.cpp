#include "../include/jetplusplus/server/server.hpp"

namespace jetpp
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
        std::string requestBody;

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
                            size_t bodyLength = 0;

                            try {
                                size_t startPos = contentLengthPos + 16;
                                size_t length = bodyStart - startPos;
                                
                                if (length > 0) {
                                    std::string lengthStr = request.substr(startPos, length);
                                    bodyLength = std::stoul(lengthStr); // Use stoul for unsigned integers
                                } else {
                                    std::cerr << "Invalid Content-Length format" << std::endl;
                                    close(clientSocket);
                                    break;
                                }
                            } catch (const std::invalid_argument& e) {
                                std::cerr << "Invalid Content-Length value: " << e.what() << std::endl;
                                close(clientSocket);
                                break;
                            } catch (const std::out_of_range& e) {
                                std::cerr << "Content-Length value out of range: " << e.what() << std::endl;
                                close(clientSocket);
                                break;
                            }

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
                        std::string clientAddress=getFullClientAddress(request);
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

    std::string Server::getFullClientAddress(const std::string& request)
    {
        // Find the "Host:" header in the request
        std::size_t hostPos = request.find("Host:");
        if (hostPos == std::string::npos) {
            std::cerr << "Host name not found in request header" << std::endl;
            return "";
        }

        // Find the end of the line containing the host information
        std::size_t endOfLine = request.find("\r\n", hostPos);
        if (endOfLine == std::string::npos) {
            std::cerr << "Invalid request format" << std::endl;
            return "";
        }

        // Extract the host information from the request
        std::string hostInfo = request.substr(hostPos, endOfLine - hostPos);

        // Extract the host name from the host information
        std::size_t colonPos = hostInfo.find(":");
        if (colonPos == std::string::npos) {
            // If no port is specified, the host information is just the host name
            return hostInfo.substr(6); // 6 is the length of "Host: "
        } else {
            // If port is specified, extract the host name before the colon
            return hostInfo.substr(6, colonPos - 6); // 6 is the length of "Host: "
        }
    }


}