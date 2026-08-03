#include "Banner.hpp"

#include <fstream>
#include <sstream>

std::string Banner::loadBanner(const std::string &path) const
{
    std::ifstream file(path);

    if (!file)
    {
        return "+==========================================+\n"
               "|          DIGITAL CLOCK SYSTEM            |\n"
               "+==========================================+\n";
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}