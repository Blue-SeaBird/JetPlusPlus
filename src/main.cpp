#include "router/router.hpp"
#include "server.hpp"
#include "request.hpp"
#include "response.hpp"
#include <iostream>
#include "json/value.hpp"

int main()
{
    // g++ -o .\main.exe main.cpp router/Router.cpp Server.cpp Response.cpp Request.cpp router/Route.cpp methods/methods.cpp json/value.cpp -lws2_32
    JETPP::Router router;

    // Define a route with a callback function
    router.get("/users", [&](JETPP::Request &req, JETPP::Response &res)
               {
        JETPP::JsonValue jsonResponse;
        JETPP::JsonValue json2;
        JETPP::JsonValue json3;
        json3.setObject({{"message3", JETPP::JsonValue("Hello World! from messages")}});
        json2.setObject({ {"message2",json3 }});
        jsonResponse.setObject({{"message", json2} });
        res.status(200).json(jsonResponse); });

    router.get("/users/:user", [&](JETPP::Request &req, JETPP::Response &res)
               { 
                    std::string message="Success for users and :user= "+req.params["user"];
                    res.send(message); });

    router.get("/users/:user/:password", [&](JETPP::Request &req, JETPP::Response &res)
               { 
                    std::string user=req.params["user"];
                    std::string password=req.params["password"];
                    std::string message="Success for users and :user= "+ user+"and his/her :password= "+password;
                    res.send(message); });

    router.get("/players", [&](JETPP::Request &req, JETPP::Response &res)
               { 
                    std::string name=req.query["name"];
                    std::string id=req.query["id"];
                    res.send("Success for player: "+name+" with id: "+id); });
    router.post("/players", [&](JETPP::Request &req, JETPP::Response &res)
                {
                    JETPP::JsonValue jsonResponse;
        JETPP::JsonValue json2;
        JETPP::JsonValue json3;
        json3.setObject({{"message3", JETPP::JsonValue("Hello World! from messages")}});
        json2.setObject({ {"message2",json3 }});
        jsonResponse.setObject({{"message", json2} });
        res.json(jsonResponse); });

    router.get("/file", [&](JETPP::Request &req, JETPP::Response &res)
               { res.sendFile("./index.html"); });

    router.get("/object", [&](JETPP::Request &req, JETPP::Response &res) {});
    // Start the server
    JETPP::Server server(router);
    server.start(8000);

    return 0;
}
