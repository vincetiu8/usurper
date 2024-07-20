#include "src/utils/time/time.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("test date", "[utils][time]") {
  Date date1 = Date(2000, 1, 1);
  Date date2 = Date(2000, 1, 2);
  Date date3 = Date(2000, 2, 1);

  SECTION("test equality") {
    Date date4 = Date(2000, 1, 1);

    REQUIRE(date1 != date2);
    REQUIRE(date1 != date3);
    REQUIRE(date1 == date4);
    REQUIRE(date2 != date3);
  }

  SECTION("test date to string") {
    REQUIRE(date1.to_yy_mm_dd_string() == "00-01-01");
    REQUIRE(date1.to_yyyy_mm_dd_string() == "2000-01-01");

    REQUIRE(date2.to_yy_mm_dd_string() == "00-01-02");
    REQUIRE(date2.to_yyyy_mm_dd_string() == "2000-01-02");

    REQUIRE(date3.to_yy_mm_dd_string() == "00-02-01");
    REQUIRE(date3.to_yyyy_mm_dd_string() == "2000-02-01");
  }

  SECTION("test date from string") {
    date1 = Date("2000-01-01", "%Y-%m-%d");
    REQUIRE(date1.year == 2000);
    REQUIRE(date1.month == 1);
    REQUIRE(date1.date == 1);

    date2 = Date("2000-01-02", "%Y-%m-%d");
    REQUIRE(date2.year == 2000);
    REQUIRE(date2.month == 1);
    REQUIRE(date2.date == 2);

    date3 = Date("2000-02-01", "%Y-%m-%d");
    REQUIRE(date3.year == 2000);
    REQUIRE(date3.month == 2);
    REQUIRE(date3.date == 1);
  }
}
