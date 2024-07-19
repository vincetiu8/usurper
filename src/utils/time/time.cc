#include "src/utils/time/time.h"
#include <ctime>
#include <string>

Date::Date(int year, int month, int date, int day)
    : year(year), month(month), date(date), day(day) {}

Date::Date(const std::string &date_str, const std::string &format) {
  tm tm = {};
  strptime(date_str.c_str(), format.c_str(), &tm);
  year = tm.tm_year + 1900;
  month = tm.tm_mon + 1;
  date = tm.tm_mday;
}

std::string Date::to_yyyy_mm_dd_string() {
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

Time::Time(int hour, int minute) : hour(hour), minute(minute) {}

Time::Time(const std::string &time_str, const std::string &format) {
  tm tm = {};
  strptime(time_str.c_str(), format.c_str(), &tm);
  hour = tm.tm_hour;
  minute = tm.tm_min;
}

std::string Time::to_hh_mm_string() {
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
