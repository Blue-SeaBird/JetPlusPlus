#include "jetplusplus/router/router.hpp"
#include "jetplusplus/server/server.hpp"
#include "jetplusplus/json/value.hpp"
#include "jetplusplus/json/jsonConverter.hpp"
#include <string>

int main(){
    jetpp::Router router;

    router.get("/test",[](jetpp::Request &req, jetpp::Response &res){
        
        res.status(200).send("Hi");
    });

    router.post("/test",[](jetpp::Request &req, jetpp::Response &res){
        jetpp::JsonConverter jsonConverter;
        std::string body=req.body;

        jetpp::JsonValue value=jsonConverter.stringToJson(body);

        res.send(std::to_string(value.asArray.size()));
    });

    jetpp::Server server(router);
    server.start(8003);
    return 0;
}