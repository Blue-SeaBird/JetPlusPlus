#include <string>

namespace JETPP
{
    enum Methods
    {
        Get,
        Post,
        Patch,
        Delete
    };
    Methods stringToMethod(std::string method);

}