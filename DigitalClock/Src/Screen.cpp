#include "Screen.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>

#include "Utility.hpp"

namespace
{
    /** Layout width used until resize() reports the real terminal size. */
    const int DEFAULT_WIDTH = 80;

    /** Layout height used until resize() reports the real terminal size. */
    const int DEFAULT_HEIGHT = 24;

    /** Smallest width the layout still renders sensibly at. */
    const int MINIMUM_WIDTH = 20;

    /** Below this height the banner is dropped to keep the clock visible. */
    const int BANNER_HEIGHT_THRESHOLD = 18;

    /** Erases from the cursor to the end of the current line. */
    const char *const ERASE_LINE = "\033[K";

    /** Erases from the cursor to the end of the screen. */
    const char *const ERASE_BELOW = "\033[J";
}

Screen::Screen()
    : layoutWidth(DEFAULT_WIDTH),
      layoutHeight(DEFAULT_HEIGHT),
      console(nullptr),
      theme(nullptr)
{
    footerHint = "Press Q or Ctrl+C to Exit";
}

void Screen::attach(Console &target)
{
    console = &target;
}

void Screen::attach(ThemeManager &source)
{
    theme = &source;
}

Banner &Screen::banner()
{
    return bannerArt;
}

void Screen::setTime(const std::string &time)
{
    timeText = time;
}

void Screen::setDate(const std::string &date)
{
    dateText = date;
}

void Screen::setStatusLines(const std::vector<std::string> &lines)
{
    statusLines = lines;
}

void Screen::setFooterHint(const std::string &hint)
{
    footerHint = hint;
}

void Screen::setNotification(const std::vector<std::string> &lines)
{
    alertLines = lines;
}

bool Screen::hasNotification() const
{
    return !alertLines.empty();
}

void Screen::resize(int width, int height)
{
    layoutWidth = (width < MINIMUM_WIDTH) ? MINIMUM_WIDTH : width;
    layoutHeight = (height < 1) ? 1 : height;

    // A short terminal cannot fit the banner and the clock at once.
    if (layoutHeight < BANNER_HEIGHT_THRESHOLD)
        bannerArt.hide();
    else
        bannerArt.show();
}

void Screen::reset()
{
    layoutWidth = DEFAULT_WIDTH;
    layoutHeight = DEFAULT_HEIGHT;

    timeText.clear();
    dateText.clear();
    statusLines.clear();
    alertLines.clear();

    footerHint = "Press Q or Ctrl+C to Exit";

    bannerArt.reset();
}

int Screen::width() const
{
    return layoutWidth;
}

int Screen::height() const
{
    return layoutHeight;
}

std::string Screen::rule(char character) const
{
    return std::string(static_cast<std::size_t>(layoutWidth), character);
}

std::vector<std::string> Screen::compose() const
{
    std::vector<std::string> lines;

    /*
    ASCII art has to be offset as a single block. Centring each line on its
    own width would shear the artwork, because the lines are deliberately of
    different lengths.
    */
    const std::vector<std::string> art = bannerArt.lines();

    std::size_t artWidth = 0;

    for (const std::string &line : art)
        artWidth = std::max(artWidth, line.size());

    const std::size_t artIndent =
        (artWidth < static_cast<std::size_t>(layoutWidth))
            ? (static_cast<std::size_t>(layoutWidth) - artWidth) / 2
            : 0;

    for (const std::string &line : art)
        lines.push_back(std::string(artIndent, ' ') + line);

    if (!lines.empty())
        lines.push_back("");

    lines.push_back("");
    lines.push_back(Utility::center(timeText, layoutWidth));
    lines.push_back("");
    lines.push_back(Utility::center(dateText, layoutWidth));
    lines.push_back("");

    // A ringing alarm sits directly under the clock, where the eye already is.
    for (const std::string &alert : alertLines)
        lines.push_back(alert);

    if (!alertLines.empty())
        lines.push_back("");

    lines.push_back(rule('-'));

    for (const std::string &status : statusLines)
        lines.push_back(" " + status);

    lines.push_back(rule('='));

    if (!footerHint.empty())
        lines.push_back(" " + footerHint);

    return lines;
}

void Screen::draw()
{
    const std::vector<std::string> lines = compose();

    /*
    Build the whole frame first, then emit it with a single write. Repainting
    in one shot keeps the terminal from showing a half-drawn screen.
    */
    std::ostringstream frame;

    // Return home rather than clearing, so nothing flashes between frames.
    frame << "\033[H";

    const std::size_t bannerLines =
        bannerArt.isVisible() ? bannerArt.lineCount() : 0;

    for (std::size_t index = 0; index < lines.size(); ++index)
    {
        const std::string &line = lines[index];

        std::string color;
        std::string clear;

        if (theme != nullptr)
        {
            ThemeManager::Element element = ThemeManager::Element::Status;

            if (index < bannerLines)
                element = ThemeManager::Element::Header;
            else if (line == rule('-') || line == rule('='))
                element = ThemeManager::Element::Accent;
            else if (!timeText.empty() &&
                     line.find(timeText) != std::string::npos)
                element = ThemeManager::Element::Time;
            else if (!dateText.empty() &&
                     line.find(dateText) != std::string::npos)
                element = ThemeManager::Element::Date;
            else if (index + 1 == lines.size() && !footerHint.empty())
                element = ThemeManager::Element::Footer;

            // The alert panel overrides every other classification.
            if (!alertLines.empty() &&
                std::find(alertLines.begin(), alertLines.end(), line) !=
                    alertLines.end())
            {
                element = ThemeManager::Element::Alert;
            }

            color = theme->colorFor(element);
            clear = color.empty() ? "" : theme->reset();
        }

        frame << color << line << clear << ERASE_LINE << '\n';
    }

    // Remove anything left over from a previously taller frame.
    frame << ERASE_BELOW;

    if (console != nullptr)
    {
        console->print(frame.str());
        console->refresh();
    }
    else
    {
        std::cout << frame.str();
        std::cout.flush();
    }
}
