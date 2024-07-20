#include "src/reserver/reserver_models/booking.h"
#include "src/reserver/reserver_models/restaurant.h"
#include "src/reserver/reserver_models/timeslot.h"
#include "src/reserver/reserver_models/user.h"
#include <catch2/catch_test_macros.hpp>
#include <optional>

struct BookingAssertion {
  int user_id;
  int timeslot_id;
};

void assert_booking(const Booking &booking, const BookingAssertion &assertion) {
  REQUIRE(booking.user_id == assertion.user_id);
  REQUIRE(booking.timeslot_id == assertion.timeslot_id);
}

void assert_booking_list(const std::vector<Booking> &bookings,
                         const std::vector<BookingAssertion> &assertions) {
  REQUIRE(bookings.size() == assertions.size());
  for (size_t i = 0; i < bookings.size(); i++) {
    assert_booking(bookings[i], assertions[i]);
  }
}

void assert_booking_list_in_db(
    const std::vector<BookingAssertion> &assertions) {
  std::vector<Booking> bookings = Booking::get_all();
  assert_booking_list(bookings, assertions);
}

TEST_CASE("booking interacts with db correctly",
          "[reserver][reserver_models][booking]") {
  Booking::drop_table();
  Timeslot::drop_table();
  Restaurant::drop_table();
  User::drop_table();
  User::create_table();
  Restaurant::create_table();
  Timeslot::create_table();
  Booking::create_table();

  User user1 = User{.name = "user 1"};
  user1.save();
  User user2 = User{.name = "user 2"};
  user2.save();

  Restaurant restaurant1 = Restaurant{.name = "restaurant 1"};
  restaurant1.save();
  Restaurant restaurant2 = Restaurant{.name = "restaurant 2"};
  restaurant2.save();

  Date date1(2000, 1, 1);
  Time start_time1(12, 0);
  Time end_time1(13, 30);

  Date date2(2000, 1, 2);
  Time start_time2(13, 0);
  Time end_time2(14, 30);

  Timeslot timeslot1 = Timeslot{
      .restaurant_id = restaurant1.id,
      .date = date1,
      .start_time = start_time1,
      .end_time = end_time1,
      .party_size = 2,
      .available = true,
  };
  timeslot1.save();
  Timeslot timeslot2 = Timeslot{
      .restaurant_id = restaurant2.id,
      .date = date2,
      .start_time = start_time2,
      .end_time = end_time2,
      .party_size = 4,
      .available = true,
  };
  timeslot2.save();

  SECTION("stores and loads booking from db") {
    Booking booking{
        .user_id = user1.id,
        .timeslot_id = timeslot1.id,
    };
    booking.create();

    BookingAssertion expected = {
        .user_id = user1.id,
        .timeslot_id = timeslot1.id,
    };

    std::optional<Booking> other =
        Booking::get(booking.user_id, booking.timeslot_id);
    REQUIRE(other.has_value());
    assert_booking(other.value(), expected);
  }

  SECTION("stores and loads multiple bookings from db") {
    Booking booking1{
        .user_id = user1.id,
        .timeslot_id = timeslot1.id,
    };
    booking1.create();
    Booking booking2{
        .user_id = user2.id,
        .timeslot_id = timeslot2.id,
    };
    booking2.create();

    std::vector<BookingAssertion> expected = {
        {.user_id = user1.id, .timeslot_id = timeslot1.id},
        {.user_id = user2.id, .timeslot_id = timeslot2.id},
    };

    assert_booking_list_in_db(expected);
  }

  SECTION("gets bookings by user id from db") {
    Booking booking1{
        .user_id = user1.id,
        .timeslot_id = timeslot1.id,
    };
    booking1.create();
    Booking booking2{
        .user_id = user2.id,
        .timeslot_id = timeslot2.id,
    };
    booking2.create();

    BookingAssertion expected = {
        .user_id = user1.id,
        .timeslot_id = timeslot1.id,
    };

    std::vector<Booking> bookings = Booking::get_by_user_id(user1.id);
    assert_booking_list(bookings, {expected});
  }

  SECTION("deletes booking from db") {
    Booking booking{
        .user_id = user1.id,
        .timeslot_id = timeslot1.id,
    };
    booking.create();

    booking.remove();

    std::vector<BookingAssertion> expected = {};
    assert_booking_list_in_db(expected);
  }

  SECTION("deletes booking by user id and timeslot id from db") {
    Booking booking{
        .user_id = user1.id,
        .timeslot_id = timeslot1.id,
    };
    booking.create();

    Booking::remove(booking.user_id, booking.timeslot_id);

    std::vector<BookingAssertion> expected = {};
    assert_booking_list_in_db(expected);
  }

  SECTION("deletes booking by user id from db") {
    Booking booking1{
        .user_id = user1.id,
        .timeslot_id = timeslot1.id,
    };
    booking1.create();
    Booking booking2{
        .user_id = user1.id,
        .timeslot_id = timeslot2.id,
    };
    booking2.create();

    Booking::remove_by_user_id(user1.id);

    std::vector<BookingAssertion> expected = {};
    assert_booking_list_in_db(expected);
  }

  SECTION("deletes all bookings from db") {
    Booking booking1{
        .user_id = user1.id,
        .timeslot_id = timeslot1.id,
    };
    booking1.create();
    Booking booking2{
        .user_id = user2.id,
        .timeslot_id = timeslot2.id,
    };
    booking2.create();

    Booking::remove_all();

    std::vector<BookingAssertion> expected = {};
    assert_booking_list_in_db(expected);
  }

  Booking::drop_table();
  Timeslot::drop_table();
  Restaurant::drop_table();
  User::drop_table();
}