#include "jetplusplus/router/router.hpp"
#include "jetplusplus/server/server.hpp"

int main(){
    JETPP::Router router;
    router.get("/test",[](JETPP::Request &req, JETPP::Response &res){
        res.send("Hello World");
    });

    JETPP::Server server(router);
    server.start(8003);
    return 0;
}