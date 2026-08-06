#include "Banner.hpp"

#include <sstream>

namespace
{
    /** Artwork used when no banner resource can be loaded. */
    const char *const DEFAULT_BANNER =
        "============================================================\n"
        "                 DIGITAL CLOCK SYSTEM\n"
        "============================================================";
}

Banner::Banner()
    : text(DEFAULT_BANNER),
      visible(true)
{
}

bool Banner::load(const std::string &fileName)
{
    if (!resources.load(fileName))
        return false;

    const std::string loaded = resources.getContent();

    // An empty file is treated as a missing one so the header is never blank.
    if (loaded.empty())
        return false;

    text = loaded;

    return true;
}

void Banner::show()
{
    visible = true;
}

void Banner::hide()
{
    visible = false;
}

bool Banner::isVisible() const
{
    return visible;
}

std::string Banner::content() const
{
    if (!visible)
        return "";

    return text;
}

std::vector<std::string> Banner::lines() const
{
    std::vector<std::string> result;

    if (!visible)
        return result;

    std::istringstream stream(text);
    std::string line;

    while (std::getline(stream, line))
    {
        // Tolerate resource files saved with Windows line endings.
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        result.push_back(line);
    }

    while (!result.empty() && result.back().empty())
        result.pop_back();

    return result;
}

std::size_t Banner::lineCount() const
{
    return lines().size();
}

void Banner::reset()
{
    text = DEFAULT_BANNER;
    visible = true;
}
