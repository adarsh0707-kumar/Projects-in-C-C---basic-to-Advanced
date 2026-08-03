#ifndef BANNER_HPP
#define BANNER_HPP

#include <string>

class Banner
{
public:
    Banner() = default;

    std::string loadBanner(const std::string &path) const;
};

#endif // BANNER_HPP