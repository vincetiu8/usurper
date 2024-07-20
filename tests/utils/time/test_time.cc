#include "src/utils/time/time.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("test time", "[utils][time]") {
  Time time1 = Time(12, 0);
  Time time2 = Time(12, 1);
  Time time3 = Time(0, 0);

  SECTION("test equality") {
    Time time4 = Time(12, 0);

    REQUIRE(time1 != time2);
    REQUIRE(time1 != time3);
    REQUIRE(time1 == time4);
    REQUIRE(time2 != time3);
  }

  SECTION("test time to string") {
    REQUIRE(time1.to_hh_mm_string() == "12:00");
    REQUIRE(time1.to_hh_mm_ss_string() == "12:00:00");

    REQUIRE(time2.to_hh_mm_string() == "12:01");
    REQUIRE(time2.to_hh_mm_ss_string() == "12:01:00");

    REQUIRE(time3.to_hh_mm_string() == "00:00");
    REQUIRE(time3.to_hh_mm_ss_string() == "00:00:00");
  }

  SECTION("test time from string") {
    time1 = Time("12:00", "%H:%M");
    REQUIRE(time1.hour == 12);
    REQUIRE(time1.minute == 0);

    time2 = Time("12:01", "%H:%M");
    REQUIRE(time2.hour == 12);
    REQUIRE(time2.minute == 1);

    time3 = Time("00:00", "%H:%M");
    REQUIRE(time3.hour == 0);
    REQUIRE(time3.minute == 0);
  }
}
