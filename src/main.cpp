#include "router/router.hpp"
#include "server.hpp"
#include "request.hpp"
#include "response.hpp"
#include <iostream>

class Person
{
public:
    std::string name;
    int age;
    Person(std::string name, int age)
    {
        this->name = name;
        this->age = age;
    }
};
int main()
{
    // g++ -o D:\Jetpp\src\main.exe main.cpp router/Router.cpp Server.cpp Response.cpp Request.cpp router/Route.cpp methods/methods.cpp -lws2_32
    JETPP::Router router;

    // Define a route with a callback function
    router.get("/users", [&](JETPP::Request &req, JETPP::Response &res)
               { 
                    std::string message="Success for users";
                    res.send(message); });

    router.post("/players", [&](JETPP::Request &req, JETPP::Response &res)
                { res.send("Success for players"); });
    router.get("/players", [&](JETPP::Request &req, JETPP::Response &res)
               { res.send("Success for players"); });

    router.get("/file", [&](JETPP::Request &req, JETPP::Response &res)
               { res.sendFile("D:\\Jetpp\\src\\index.html"); });

    router.get("/object", [&](JETPP::Request &req, JETPP::Response &res)
               { 
                Person person("Simon",17);
                res.send(person); });

    // Start the server
    JETPP::Server server(router);
    server.start(8000);

    return 0;
}
