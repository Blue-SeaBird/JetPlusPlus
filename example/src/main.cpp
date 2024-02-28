#include "jetplusplus/router/router.hpp"
#include "jetplusplus/server/server.hpp"

int main(){
    JETPP::Router router;
    router.post("/test",[](JETPP::Request &req, JETPP::Response &res){
        res.send(req.body);
    });

    JETPP::Server server(router);
    server.start(8003);
    return 0;
}