#include "src/utils/time/time.h"
#include <ctime>
#include <string>

Date::Date(int year, int month, int date)
    : year{year}, month{month}, date{date} {}

Date::Date(const std::string &date_str, const std::string &format) {
  tm tm = {};
  strptime(date_str.c_str(), format.c_str(), &tm);
  year = tm.tm_year + 1900;
  month = tm.tm_mon + 1;
  date = tm.tm_mday;
}

std::string Date::to_yyyy_mm_dd_string() const {
  std::string s = std::to_string(year) + "-";
  if (month < 10) {
    s += "0";
  }
  s += std::to_string(month) + "-";
  if (date < 10) {
    s += "0";
  }
  s += std::to_string(date);
  return s;
}

std::string Date::to_yy_mm_dd_string() const {
  std::string s = "";
  if (year == 2000) {
    s += "0";
  }
  s += std::to_string(year % 100) + "-";
  if (month < 10) {
    s += "0";
  }
  s += std::to_string(month) + "-";
  if (date < 10) {
    s += "0";
  }
  s += std::to_string(date);
  return s;
}

bool Date::operator==(const Date &rhs) const {
  return year == rhs.year && month == rhs.month && date == rhs.date;
}

bool Date::operator!=(const Date &rhs) const { return !(*this == rhs); }

Time::Time(int hour, int minute) : hour{hour}, minute{minute} {}

Time::Time(const std::string &time_str, const std::string &format) {
  tm tm = {};
  strptime(time_str.c_str(), format.c_str(), &tm);
  hour = tm.tm_hour;
  minute = tm.tm_min;
}

std::string Time::to_hh_mm_string() const {
  std::string s = "";
  if (hour < 10) {
    s += "0";
  }
  s += std::to_string(hour) + ":";
  if (minute < 10) {
    s += "0";
  }
  s += std::to_string(minute);
  return s;
}

std::string Time::to_hh_mm_ss_string() const {
  std::string s = "";
  if (hour < 10) {
    s += "0";
  }
  s += std::to_string(hour) + ":";
  if (minute < 10) {
    s += "0";
  }
  s += std::to_string(minute) + ":00";
  return s;
}

bool Time::operator==(const Time &rhs) const {
  return hour == rhs.hour && minute == rhs.minute;
}

bool Time::operator!=(const Time &rhs) const { return !(*this == rhs); }
