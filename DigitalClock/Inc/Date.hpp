#ifndef DATE_HPP
#define DATE_HPP

#include <string>

class Date
{
public:
    Date();

    void update();

    int getDay() const;
    int getMonth() const;
    int getYear() const;

    std::string getWeekday() const;
    std::string getMonthName() const;

    std::string getDateNumeric() const;
    std::string getDateLong() const;

private:
    int day;
    int month;
    int year;

    int weekday;
};

#endif // DATE_HPP