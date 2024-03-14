#include "jetplusplus/router/router.hpp"
#include "jetplusplus/server/server.hpp"
#include "jetplusplus/json/value.hpp"
#include "jetplusplus/json/jsonConverter.hpp"
#include <string>

int main(){
    JETPP::Router router;

    router.get("/test",[](JETPP::Request &req, JETPP::Response &res){
        
        res.status(200).send("Hi");
    });

    router.post("/test",[](JETPP::Request &req, JETPP::Response &res){
        JETPP::JsonConverter jsonConverter;
        std::string body=req.body;

        JETPP::JsonValue value=jsonConverter.stringToJson(body);

        res.send(std::to_string(value.asArray.size()));
    });

    JETPP::Server server(router);
    server.start(8003);
    return 0;
}