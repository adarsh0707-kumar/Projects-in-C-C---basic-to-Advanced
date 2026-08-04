#ifndef BANNER_HPP
#define BANNER_HPP

/*
------------------------------------------------------------
 Banner Module
------------------------------------------------------------

Purpose
-------
Responsible for loading ASCII text resources used by the
application UI.

The Banner module separates presentation resources from
the source code.

Resources
---------
Resources/logo.txt
    Large ASCII logo shown at application startup.

Resources/banner.txt
    Compact banner displayed during every screen refresh.

Author
------
Adarsh Kumar

------------------------------------------------------------
*/

#include <string>

class Banner
{
public:
    /*
    --------------------------------------------------------
    Constructor
    --------------------------------------------------------
    Default constructor.

    No initialization is required because this class simply
    reads text files from disk.
    --------------------------------------------------------
    */
    Banner() = default;

    /*
    --------------------------------------------------------
    Load startup logo.

    Returns:
        Complete contents of Resources/logo.txt

    If the file cannot be opened,
    a default logo is returned.
    --------------------------------------------------------
    */
    std::string loadLogo() const;

    /*
    --------------------------------------------------------
    Load application banner.

    Returns:
        Complete contents of Resources/banner.txt

    Used by Display every time the screen refreshes.

    If the file is missing,
    a default banner is returned.
    --------------------------------------------------------
    */
    std::string loadBanner() const;

private:
    /*
    --------------------------------------------------------
    Read any text file.

    Parameter:
        filename

    Returns:
        Entire file as std::string.

    Used internally by loadLogo()
    and loadBanner().
    --------------------------------------------------------
    */
    std::string readFile(const std::string &filename) const;
};

#endif