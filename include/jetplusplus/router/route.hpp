#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <string>
#include <functional>
#include "../server/request.hpp"
#include "../server/response.hpp"
#include "../methods/methods.hpp"

namespace JETPP
{
    class Route
    {
    public:
        Route(std::string name, JETPP::Methods method, void (*callback)(Request &, Response &));

        std::string getName();
        JETPP::Methods getMethod();
        void execute(Request &request, Response &response);

    private:
        std::string name;
        JETPP::Methods method;
        void (*callback)(Request &, Response &);
    };
}

#endif
