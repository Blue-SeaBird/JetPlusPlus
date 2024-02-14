#ifndef SERVER_HPP
#define SERVER_HPP

#include "router/router.hpp"
#include "request.hpp"
#include "response.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>

namespace JETPP
{
    class Server
    {
    private:
        Router router;
        int port;
        void handleClient(int clientSocket);

    public:
        Server(Router router);
        void start(int port);
        void sendResponse(int clientSocket, const char *response); // Change SOCKET to int
    };
}

#endif
