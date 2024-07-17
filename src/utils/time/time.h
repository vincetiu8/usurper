#pragma once

#include <chrono>
#include <string>

namespace chrono = std::chrono;

class Date {
public:
  Date(int year, int month, int date, int day);

  Date(const std::string &date_str, const std::string &format);

  // year
  int year;

  // month
  // begins on January
  int month;

  // date of the month
  int date;

  // day of the week
  // begins on monday
  int day;

  std::string to_yyyy_mm_dd_string();
};

class Time {
public:
  Time(int hour, int minute);

  Time(const std::string &time_str, const std::string &format);

  int hour;

  int minute;

  std::string to_hh_mm_string();
};
