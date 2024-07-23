#pragma once

#include <string>

class Date {
public:
  // year
  int year;

  // month
  // begins on January
  int month;

  // date of the month
  int date;

  Date(int year, int month, int date);

  Date(const std::string &date_str, const std::string &format);

  std::string to_yyyy_mm_dd_string() const;

  std::string to_yy_mm_dd_string() const;

  bool operator==(const Date &rhs) const;

  bool operator!=(const Date &rhs) const;
};

class Time {
public:
  int hour;

  int minute;

  Time(int hour, int minute);

  Time(const std::string &time_str, const std::string &format);

  std::string to_hh_mm_string() const;

  std::string to_hh_mm_ss_string() const;

  bool operator==(const Time &rhs) const;

  bool operator!=(const Time &rhs) const;

  Time operator+(const Time &rhs) const;

  Time operator+=(const Time &rhs);

  Time operator-(const Time &rhs) const;

  Time operator-=(const Time &rhs);

  bool operator<(const Time &rhs) const;

  bool operator>(const Time &rhs) const;

  bool operator<=(const Time &rhs) const;

  bool operator>=(const Time &rhs) const;
};
