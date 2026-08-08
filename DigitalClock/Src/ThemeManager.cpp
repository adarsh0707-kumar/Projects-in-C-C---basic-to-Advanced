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
      applied(false),
      unknownTokens(0)
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
    colors[Element::Alert]  = Theme::Color::BrightYellow;
    colors[Element::Error]  = Theme::Color::Red;

    // The default theme is plain: styles are something a theme opts into.
    elementStyles.clear();

    themeName = DEFAULT_THEME_NAME;
}

std::size_t ThemeManager::parseAppearance(
    const std::string &value,
    Theme::Color &color,
    std::vector<Theme::Style> &parsedStyles)
{
    parsedStyles.clear();

    std::size_t unrecognised = 0;

    // Commas are accepted so "Cyan, Bold" reads naturally in a theme file.
    std::string token;
    std::string remaining = Utility::trim(value) + " ";

    for (const char character : remaining)
    {
        if (character != ' ' && character != '\t' && character != ',')
        {
            token += character;
            continue;
        }

        if (token.empty())
            continue;

        /*
        Styles are tested first. No name is both a colour and a style, so the
        order does not change the result; testing styles first just keeps the
        colour fallback from swallowing a style name.
        */
        Theme::Style style = Theme::Style::Normal;
        Theme::Color named = Theme::Color::Default;

        if (Theme::parseStyle(token, style))
        {
            // "Normal" clears rather than accumulating, so a theme can say so.
            if (style == Theme::Style::Normal)
                parsedStyles.clear();
            else
                parsedStyles.push_back(style);
        }
        else if (Theme::parseColor(token, named))
        {
            color = named;
        }
        else
        {
            ++unrecognised;
        }

        token.clear();
    }

    return unrecognised;
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

    if (name == "ALERT" || name == "ALARM")
    {
        element = Element::Alert;
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

    unknownTokens = 0;

    static const std::pair<const char *, Element> KEYS[] = {
        {"HEADER", Element::Header},
        {"TIME", Element::Time},
        {"DATE", Element::Date},
        {"STATUS", Element::Status},
        {"FOOTER", Element::Footer},
        {"ACCENT", Element::Accent},
        {"ALERT", Element::Alert},
        {"ERROR", Element::Error}};

    for (const auto &entry : KEYS)
    {
        if (!definition.hasValue(entry.first))
            continue;

        Element element = Element::Header;

        if (!elementFromKey(entry.first, element))
            continue;

        Theme::Color parsed = colors[element];
        std::vector<Theme::Style> parsedStyles;

        unknownTokens += parseAppearance(
            definition.getValue(entry.first), parsed, parsedStyles);

        colors[element] = parsed;

        if (parsedStyles.empty())
            elementStyles.erase(element);
        else
            elementStyles[element] = parsedStyles;

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

std::vector<Theme::Style> ThemeManager::styles(Element element) const
{
    const auto entry = elementStyles.find(element);

    if (entry == elementStyles.end())
        return {};

    return entry->second;
}

std::string ThemeManager::describe(Element element) const
{
    std::string text = Theme::colorName(color(element));

    for (const Theme::Style style : styles(element))
        text += " " + Theme::styleName(style);

    return text;
}

std::size_t ThemeManager::unknownTokenCount() const
{
    return unknownTokens;
}

std::string ThemeManager::colorFor(Element element) const
{
    if (!colorEnabled)
        return "";

    /*
    Styles precede the colour. Both are plain SGR sequences and either order
    renders the same, but writing the attribute first matches how the reset
    at the end of the run reads: attributes on, text, everything off.
    */
    std::string sequence;

    for (const Theme::Style style : styles(element))
        sequence += Theme::style(style);

    return sequence + Theme::foreground(color(element));
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

std::string ThemeManager::cycleTheme()
{
    const std::vector<std::string> names = availableThemes();

    if (names.empty())
        return currentTheme();

    const std::string active = Utility::toLower(themeName);

    std::size_t next = 0;

    for (std::size_t index = 0; index < names.size(); ++index)
    {
        if (Utility::toLower(names[index]) != active)
            continue;

        next = (index + 1) % names.size();
        break;
    }

    /*
    A theme outside the shipped list -- the built-in default, or a file the
    user added -- leaves next at 0, so the cycle starts from the beginning
    rather than refusing to move.
    */
    loadTheme(names[next]);

    return currentTheme();
}
