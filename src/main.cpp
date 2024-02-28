#include "../include/router/router.hpp"
#include "../include/server/server.hpp"
#include "../include/server/request.hpp"
#include "../include/server/response.hpp"
#include "../include/container/container.hpp"
#include <iostream>
#include <filesystem>
#include "../include/json/value.hpp"

int main()
{
    JETPP::Router router;

    // Define a route with a callback function
    router.get("/users", [](JETPP::Request &req, JETPP::Response &res)
               {
        JETPP::JsonValue jsonResponse;
        JETPP::JsonValue json2;
        JETPP::JsonValue json3;
        json3.setObject({{"message3", JETPP::JsonValue("Hello World! from messages")}});
        json2.setObject({ {"message2",json3 }});
        jsonResponse.setObject({{"message", json2} });
        res.status(200).json(jsonResponse); });

    JETPP::Container userProxyContainer;
    userProxyContainer.addAccessHost("127.0.0.1");

    router.get("/users/:user", [](JETPP::Request &req, JETPP::Response &res)
               { 
                    std::string message="Success for users and :user= "+req.params["user"];
                    res.send(message); }, userProxyContainer);

    router.get("/users/:user/:password", [](JETPP::Request &req, JETPP::Response &res)
               { 
                    std::string user=req.params["user"];
                    std::string password=req.params["password"];
                    std::string message="Success for users and :user= "+ user+"and his/her :password= "+password;
                    res.send(message); });

    router.get("/players", [](JETPP::Request &req, JETPP::Response &res)
               { 
                    std::string name=req.query["name"];
                    std::string id=req.query["id"];
                    res.send("Success for player: "+name+" with id: "+id); });

    router.post("/players", [](JETPP::Request &req, JETPP::Response &res)
                {
                    JETPP::JsonValue jsonResponse;
        JETPP::JsonValue json2;
        JETPP::JsonValue json3;
        json3.setObject({{"message3", JETPP::JsonValue("Hello World! from messages")}});
        json2.setObject({ {"message2",json3 }});
        jsonResponse.setObject({{"message", json2} });
        res.json(jsonResponse); });

    // Start the server
    JETPP::Server server(router);
    server.start(8000);

    return 0;
}
