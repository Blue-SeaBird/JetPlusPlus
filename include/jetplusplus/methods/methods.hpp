#include <string>

namespace JETPP
{
    enum Methods
    {
        Get,
        Post,
        Put,
        Patch,
        Delete,
        Options
    };
    Methods stringToMethod(std::string method);

}