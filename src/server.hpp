#ifndef SERVER_HPP
#define SERVER_HPP

#include "router/router.hpp"
#include <winsock2.h>

namespace JETPP
{
    class Server
    {
    private:
        Router router;
        int port;

    public:
        Server(Router router);
        void start(int port);
        void sendResponse(SOCKET clientSocket, const char *response);
    };
}

#endif
