#include "src/reserver/reserver_models/timeslot.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <optional>

struct TimeslotAssertion {
  int id;
  int restaurant_id;
  Date date;
  Time start_time;
  Time end_time;
  int party_size;
  bool available;
};

void assert_timeslot(const Timeslot &timeslot,
                     const TimeslotAssertion &assertion) {
  REQUIRE(timeslot.id == assertion.id);
  REQUIRE(timeslot.restaurant_id == assertion.restaurant_id);
  REQUIRE(timeslot.date == assertion.date);
  REQUIRE(timeslot.start_time == assertion.start_time);
  REQUIRE(timeslot.end_time == assertion.end_time);
  REQUIRE(timeslot.party_size == assertion.party_size);
  REQUIRE(timeslot.available == assertion.available);
}

void assert_timeslot_list(const std::vector<Timeslot> &timeslots,
                          const std::vector<TimeslotAssertion> &assertions) {
  REQUIRE(timeslots.size() == assertions.size());
  for (size_t i = 0; i < timeslots.size(); i++) {
    assert_timeslot(timeslots[i], assertions[i]);
  }
}

void assert_timeslot_list_in_db(
    const std::vector<TimeslotAssertion> &assertions) {
  std::vector<Timeslot> timeslots = Timeslot::get_all();
  assert_timeslot_list(timeslots, assertions);
}

TEST_CASE("timeslot interacts with db correctly",
          "[reserver][reserver_models][timeslot]") {
  Timeslot::drop_table();
  Timeslot::create_table();

  Date date1(2000, 1, 1);
  Time start_time1(12, 0);
  Time end_time1(13, 30);

  Date date2(2000, 1, 2);
  Time start_time2(13, 0);
  Time end_time2(14, 30);

  SECTION("stores and loads timeslot from db") {
    Timeslot timeslot{
        .restaurant_id = 1,
        .date = date1,
        .start_time = start_time1,
        .end_time = end_time1,
        .party_size = 2,
        .available = true,
    };
    timeslot.save();

    TimeslotAssertion expected = {
        .id = timeslot.id,
        .restaurant_id = 1,
        .date = date1,
        .start_time = start_time1,
        .end_time = end_time1,
        .party_size = 2,
        .available = true,
    };

    assert_timeslot_list_in_db({expected});

    std::optional<Timeslot> other = Timeslot::get(timeslot.id);
    REQUIRE(other.has_value());
    assert_timeslot(other.value(), expected);
  }

  SECTION("stores and loads multiple timeslots from db") {
    Timeslot timeslot1{
        .restaurant_id = 1,
        .date = date1,
        .start_time = start_time1,
        .end_time = end_time1,
        .party_size = 2,
        .available = true,
    };
    timeslot1.save();
    Timeslot timeslot2{
        .restaurant_id = 2,
        .date = date2,
        .start_time = start_time2,
        .end_time = end_time2,
        .party_size = 4,
        .available = false,
    };
    timeslot2.save();

    std::vector<TimeslotAssertion> expected = {
        {.id = timeslot1.id,
         .restaurant_id = 1,
         .date = date1,
         .start_time = start_time1,
         .end_time = end_time1,
         .party_size = 2,
         .available = true},
        {.id = timeslot2.id,
         .restaurant_id = 2,
         .date = date2,
         .start_time = start_time2,
         .end_time = end_time2,
         .party_size = 4,
         .available = false},
    };

    assert_timeslot_list_in_db(expected);
  }

  SECTION("updates timeslot with same reference in db") {
    Timeslot timeslot{
        .restaurant_id = 1,
        .date = date1,
        .start_time = start_time1,
        .end_time = end_time1,
        .party_size = 2,
        .available = true,
    };
    timeslot.save();

    timeslot.party_size = 4;
    timeslot.save();

    TimeslotAssertion expected = {
        .id = timeslot.id,
        .restaurant_id = 1,
        .date = date1,
        .start_time = start_time1,
        .end_time = end_time1,
        .party_size = 4,
        .available = true,
    };

    assert_timeslot_list_in_db({expected});
  }

  SECTION("updates timeslot with different reference in db") {
    Timeslot timeslot{
        .restaurant_id = 1,
        .date = date1,
        .start_time = start_time1,
        .end_time = end_time1,
        .party_size = 2,
        .available = true,
    };
    timeslot.save();

    std::optional<Timeslot> other = Timeslot::get(timeslot.id);
    REQUIRE(other.has_value());

    other.value().party_size = 4;
    other.value().save();

    TimeslotAssertion expected = {
        .id = timeslot.id,
        .restaurant_id = 1,
        .date = date1,
        .start_time = start_time1,
        .end_time = end_time1,
        .party_size = 4,
        .available = true,
    };

    assert_timeslot_list_in_db({expected});
  }

  SECTION("removes timeslot from db") {
    Timeslot timeslot{
        .restaurant_id = 1,
        .date = date1,
        .start_time = start_time1,
        .end_time = end_time1,
        .party_size = 2,
        .available = true,
    };
    timeslot.save();

    timeslot.remove();
    assert_timeslot_list_in_db({});
  }

  SECTION("removes timeslot by id from db") {
    Timeslot timeslot{
        .restaurant_id = 1,
        .date = date1,
        .start_time = start_time1,
        .end_time = end_time1,
        .party_size = 2,
        .available = true,
    };
    timeslot.save();

    Timeslot::remove(timeslot.id);
    assert_timeslot_list_in_db({});
  }

  SECTION("removes all timeslots from db") {
    Timeslot timeslot1{
        .restaurant_id = 1,
        .date = date1,
        .start_time = start_time1,
        .end_time = end_time1,
        .party_size = 2,
        .available = true,
    };
    timeslot1.save();
    Timeslot timeslot2{
        .restaurant_id = 2,
        .date = date2,
        .start_time = start_time2,
        .end_time = end_time2,
        .party_size = 4,
        .available = false,
    };
    timeslot2.save();

    Timeslot::remove_all();
    assert_timeslot_list_in_db({});
  }
}
