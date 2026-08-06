#include "ThemeManager.hpp"

#include "ConfigurationManager.hpp"
#include "Utility.hpp"

namespace
{
    /** Name reported when no theme file has been loaded successfully. */
    const char *const DEFAULT_THEME_NAME = "Default";
}

ThemeManager::ThemeManager()
    : colorEnabled(true),
      applied(false)
{
    applyDefaultTheme();
}

void ThemeManager::applyDefaultTheme()
{
    colors[Element::Header] = Theme::Color::Cyan;
    colors[Element::Time]   = Theme::Color::Green;
    colors[Element::Date]   = Theme::Color::Yellow;
    colors[Element::Status] = Theme::Color::White;
    colors[Element::Footer] = Theme::Color::BrightBlack;
    colors[Element::Accent] = Theme::Color::Blue;
    colors[Element::Error]  = Theme::Color::Red;

    themeName = DEFAULT_THEME_NAME;
}

std::string ThemeManager::themePath(const std::string &themeName)
{
    return "Resources/themes/" + Utility::toLower(Utility::trim(themeName)) +
           ".theme";
}

bool ThemeManager::elementFromKey(const std::string &key, Element &element)
{
    const std::string name = Utility::toUpper(Utility::trim(key));

    if (name == "HEADER" || name == "BANNER")
    {
        element = Element::Header;
        return true;
    }

    if (name == "TIME" || name == "CLOCK")
    {
        element = Element::Time;
        return true;
    }

    if (name == "DATE")
    {
        element = Element::Date;
        return true;
    }

    if (name == "STATUS")
    {
        element = Element::Status;
        return true;
    }

    if (name == "FOOTER")
    {
        element = Element::Footer;
        return true;
    }

    if (name == "ACCENT" || name == "SEPARATOR")
    {
        element = Element::Accent;
        return true;
    }

    if (name == "ERROR")
    {
        element = Element::Error;
        return true;
    }

    return false;
}

bool ThemeManager::loadTheme(const std::string &requestedName)
{
    const std::string name = Utility::trim(requestedName);

    if (name.empty())
    {
        applyDefaultTheme();
        return false;
    }

    const std::string path = themePath(name);

    if (!resources.exists(path))
    {
        // Unknown theme: fall back to the default (TC-013, TC-025).
        applyDefaultTheme();
        return false;
    }

    ConfigurationManager definition;

    if (!definition.load(resources.resolve(path)))
    {
        applyDefaultTheme();
        return false;
    }

    /*
    Start from the defaults so a theme file that names only some elements
    still produces a complete, usable colour set.
    */
    applyDefaultTheme();

    bool matchedAny = false;

    static const std::pair<const char *, Element> KEYS[] = {
        {"HEADER", Element::Header},
        {"TIME", Element::Time},
        {"DATE", Element::Date},
        {"STATUS", Element::Status},
        {"FOOTER", Element::Footer},
        {"ACCENT", Element::Accent},
        {"ERROR", Element::Error}};

    for (const auto &entry : KEYS)
    {
        if (!definition.hasValue(entry.first))
            continue;

        Element element = Element::Header;

        if (!elementFromKey(entry.first, element))
            continue;

        const Theme::Color parsed = Theme::colorFromName(
            definition.getValue(entry.first),
            colors[element]);

        colors[element] = parsed;
        matchedAny = true;
    }

    if (!matchedAny)
    {
        // The file existed but defined nothing recognisable (TC-023).
        applyDefaultTheme();
        return false;
    }

    // Report the name with its original capitalisation, e.g. "Dark".
    themeName = name;

    return true;
}

bool ThemeManager::applyTheme()
{
    applied = true;

    return colorEnabled;
}

std::string ThemeManager::currentTheme() const
{
    return themeName;
}

Theme::Color ThemeManager::color(Element element) const
{
    const auto entry = colors.find(element);

    if (entry == colors.end())
        return Theme::Color::Default;

    return entry->second;
}

std::string ThemeManager::colorFor(Element element) const
{
    if (!colorEnabled)
        return "";

    return Theme::foreground(color(element));
}

std::string ThemeManager::reset() const
{
    if (!colorEnabled)
        return "";

    return Theme::reset();
}

void ThemeManager::setColorEnabled(bool enabled)
{
    colorEnabled = enabled;
}

bool ThemeManager::isColorEnabled() const
{
    return colorEnabled;
}

bool ThemeManager::isApplied() const
{
    return applied;
}

std::vector<std::string> ThemeManager::availableThemes()
{
    return {"Dark", "Light", "Blue", "Green", "HighContrast"};
}
