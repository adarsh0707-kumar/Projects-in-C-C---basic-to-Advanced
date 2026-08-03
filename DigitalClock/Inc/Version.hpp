#ifndef VERSION_HPP
#define VERSION_HPP

#include <string>

namespace Version
{
    inline constexpr int MAJOR = 1;
    inline constexpr int MINOR = 0;
    inline constexpr int PATCH = 0;

    inline const std::string NAME = "Digital Clock System";

    inline const std::string AUTHOR = "Adarsh Kumar";

    inline const std::string VERSION =
        std::to_string(MAJOR) + "." +
        std::to_string(MINOR) + "." +
        std::to_string(PATCH);
}

#endif // VERSION_HPP