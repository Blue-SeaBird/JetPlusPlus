#include "router/router.hpp"
#include "server.hpp"
#include "request.hpp"
#include "response.hpp"
#include <iostream>

int main()
{
    // g++ -o D:\Jetpp\src\main.exe main.cpp router/Router.cpp Server.cpp Response.cpp Request.cpp router/Route.cpp -lws2_32
    JETPP::Router router;

    // Define a route with a callback function
    router.get("/users", [&](JETPP::Request &req, JETPP::Response &res)
               { 
                    int status=200;
                    std::string message="Success for users";
                    res.send(status, message); });

    router.post("/players", [&](JETPP::Request &req, JETPP::Response &res)
                { res.send(200, "Success for players"); });

    // Start the server
    JETPP::Server server(router);
    server.start(8000);

    return 0;
}
