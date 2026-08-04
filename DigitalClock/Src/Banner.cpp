#include "Banner.hpp"

#include <fstream>
#include <sstream>

/*
------------------------------------------------------------
Read an entire text file.

This utility function is shared by every public loader.

Parameters
----------
filename
    Path of the file to read.

Returns
-------
Entire file as one string.

If the file cannot be opened,
an empty string is returned.
------------------------------------------------------------
*/
std::string Banner::readFile(const std::string &filename) const
{
    std::ifstream file(filename);

    if (!file)
    {
        return "";
    }

    std::ostringstream buffer;

    buffer << file.rdbuf();

    return buffer.str();
}

/*
------------------------------------------------------------
Load startup logo.

Attempts to read:

Resources/logo.txt

If the file does not exist,
returns a built-in fallback logo.
------------------------------------------------------------
*/
std::string Banner::loadLogo() const
{
    std::string logo = readFile("Resources/logo.txt");

    if (!logo.empty())
    {
        return logo;
    }

    return
        R"(

============================================================

                 DIGITAL CLOCK SYSTEM

============================================================

)";
}

/*
------------------------------------------------------------
Load compact application banner.

Attempts to read:

Resources/banner.txt

If unavailable,
returns a default banner.
------------------------------------------------------------
*/
std::string Banner::loadBanner() const
{
    std::string banner = readFile("Resources/banner.txt");

    if (!banner.empty())
    {
        return banner;
    }

    return
        R"(============================================================
                 DIGITAL CLOCK SYSTEM
============================================================
)";
}