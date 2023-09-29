#include "route.hpp"
#include <string>
#include <stdio.h>

namespace JETPP
{
    Route::Route(std::string name, std::function<void(Request &, Response &)> callback)
    {
        this->name = name;
        this->callback = callback;
    }

    std::string Route::getName()
    {
        return name;
    }

    void Route::execute(Request &request, Response &response)
    {
        this->callback(request, response); // Execute the callback function
    }
}
