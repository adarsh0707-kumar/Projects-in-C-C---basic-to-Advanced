#include "Notifier.hpp"

#include <algorithm>
#include <iostream>

namespace
{
    /** Width of the alert panel when the layout allows it. */
    const int PREFERRED_WIDTH = 46;

    /** Narrowest panel that still reads correctly. */
    const int MINIMUM_WIDTH = 24;

    /**
     * @brief Pads or truncates text to exactly @p width characters.
     */
    std::string fit(const std::string &text, std::size_t width)
    {
        if (text.size() >= width)
            return text.substr(0, width);

        return text + std::string(width - text.size(), ' ');
    }
}

Notifier::Notifier()
    : active(false),
      bell(true),
      bells(0)
{
}

void Notifier::notify(const Alarm &alarm, int snoozeMinutes)
{
    active = true;
    alarmTime = alarm.timeString();
    alarmLabel = alarm.getLabel();

    hint = "[S] Snooze " + std::to_string(snoozeMinutes) +
           "m   [D] Dismiss";

    ring();
}

void Notifier::pulse()
{
    if (!active)
        return;

    ring();
}

void Notifier::ring()
{
    if (!bell)
        return;

    // BEL. Terminals that have the audible bell disabled usually flash instead.
    std::cout << '\a';

    ++bells;
}

void Notifier::clear()
{
    active = false;
    alarmTime.clear();
    alarmLabel.clear();
    hint.clear();
}

bool Notifier::isActive() const
{
    return active;
}

std::string Notifier::label() const
{
    return alarmLabel;
}

std::string Notifier::time() const
{
    return alarmTime;
}

void Notifier::setBellEnabled(bool enabled)
{
    bell = enabled;
}

bool Notifier::bellEnabled() const
{
    return bell;
}

int Notifier::bellCount() const
{
    return bells;
}

std::vector<std::string> Notifier::lines(int width) const
{
    std::vector<std::string> panel;

    if (!active)
        return panel;

    // Fit the panel to the layout, leaving a margin, without going too narrow.
    int panelWidth = std::min(PREFERRED_WIDTH, width - 4);
    panelWidth = std::max(panelWidth, MINIMUM_WIDTH);

    const std::size_t inner = static_cast<std::size_t>(panelWidth) - 4;

    const std::string border =
        "+" + std::string(static_cast<std::size_t>(panelWidth) - 2, '-') + "+";

    const int indent = std::max(0, (width - panelWidth) / 2);
    const std::string pad(static_cast<std::size_t>(indent), ' ');

    auto row = [&](const std::string &text)
    {
        panel.push_back(pad + "| " + fit(text, inner) + " |");
    };

    panel.push_back(pad + border);

    row("ALARM  " + alarmTime);

    if (!alarmLabel.empty())
        row(alarmLabel);

    row("");
    row(hint);

    panel.push_back(pad + border);

    return panel;
}
