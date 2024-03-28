#include "jetplusplus/router/router.hpp"
#include "jetplusplus/server/server.hpp"
#include "jetplusplus/json/value.hpp"
#include "jetplusplus/json/jsonConverter.hpp"

#include <string>

int main(){
    jetpp::Router router;
    std::shared_ptr<jetpp::Container> c=std::make_shared<jetpp::Container>();
    c->addAccessHost("localhost:8003");

    router.get("/test",[](jetpp::Request &req, jetpp::Response &res){
        
        res.status(200).send("Hi");
    },c);

    router.post("/test",[](jetpp::Request &req, jetpp::Response &res){
        jetpp::JsonConverter jsonConverter;
        std::string body=req.body;
        jetpp::JsonValue value=jsonConverter.stringToJson(body);

        res.json(value);
    },c);

    jetpp::Server server(router);
    server.start(8003);
    return 0;
}