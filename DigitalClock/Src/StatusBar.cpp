#include "StatusBar.hpp"

#include <algorithm>
#include <iostream>

#include "Utility.hpp"

StatusBar::StatusBar()
    : visible(true)
{
}

void StatusBar::setMessage(const std::string &message)
{
    transientMessage = message;
}

std::string StatusBar::message() const
{
    return transientMessage;
}

void StatusBar::setField(const std::string &name, const std::string &value)
{
    const std::string label = Utility::trim(name);

    if (label.empty())
        return;

    const auto existing = std::find_if(
        fields.begin(),
        fields.end(),
        [&label](const std::pair<std::string, std::string> &entry)
        { return entry.first == label; });

    if (existing != fields.end())
    {
        existing->second = value;
        return;
    }

    fields.emplace_back(label, value);
}

std::string StatusBar::field(const std::string &name) const
{
    const std::string label = Utility::trim(name);

    const auto existing = std::find_if(
        fields.begin(),
        fields.end(),
        [&label](const std::pair<std::string, std::string> &entry)
        { return entry.first == label; });

    if (existing == fields.end())
        return "";

    return existing->second;
}

std::vector<std::string> StatusBar::lines() const
{
    std::vector<std::string> rows;

    if (!visible)
        return rows;

    // Pad labels to a common width so the colons line up.
    std::size_t labelWidth = 0;

    for (const auto &entry : fields)
        labelWidth = std::max(labelWidth, entry.first.size());

    for (const auto &entry : fields)
    {
        std::string row = entry.first;
        row.append(labelWidth - entry.first.size(), ' ');
        row += " : " + entry.second;

        rows.push_back(row);
    }

    if (!transientMessage.empty())
        rows.push_back(transientMessage);

    return rows;
}

void StatusBar::show()
{
    for (const std::string &row : lines())
        std::cout << row << '\n';
}

void StatusBar::clear()
{
    fields.clear();
    transientMessage.clear();
}

void StatusBar::setVisible(bool isVisible)
{
    visible = isVisible;
}

bool StatusBar::isVisible() const
{
    return visible;
}
