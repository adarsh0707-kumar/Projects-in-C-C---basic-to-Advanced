#ifndef SCREEN_HPP
#define SCREEN_HPP

class Screen
{
public:
    Screen() = default;

    void clear() const;

    void moveCursorHome() const;
};

#endif // SCREEN_HPP