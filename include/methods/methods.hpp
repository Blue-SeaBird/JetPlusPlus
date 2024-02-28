#include <string>

namespace JETPP
{
    enum Methods
    {
        Get,
        Post,
        Patch,
        Delete,
        Options
    };
    Methods stringToMethod(std::string method);

}