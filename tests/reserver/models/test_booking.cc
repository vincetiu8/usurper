#include "src/reserver/models/booking.h"
#include "src/reserver/models/restaurant.h"
#include "src/reserver/models/timeslot.h"
#include "src/reserver/models/user.h"
#include <catch2/catch_test_macros.hpp>
#include <optional>

struct BookingAssertion {
  int user_id;
  int timeslot_id;
  std::optional<std::string> resy_token;
};

void assert_booking(const Booking &booking, const BookingAssertion &assertion) {
  REQUIRE(booking.user_id == assertion.user_id);
  REQUIRE(booking.timeslot_id == assertion.timeslot_id);
  REQUIRE(booking.resy_token == assertion.resy_token);
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
          "[reserver][models][booking]") {
  Booking::drop_table();
  Timeslot::drop_table();
  Restaurant::drop_table();
  User::drop_table();
  User::create_table();
  Restaurant::create_table();
  Timeslot::create_table();
  Booking::create_table();

  User user1 = User{.name = "user 1", .resy_token = "token 1"};
  user1.save();
  User user2 = User{.name = "user 2", .resy_token = "token 2"};
  user2.save();

  Restaurant restaurant1 = Restaurant{.name = "restaurant 1",
                                      .rsc = ReservationServiceCode::resy,
                                      .resy_id = 1};
  restaurant1.save();
  Restaurant restaurant2 = Restaurant{.name = "restaurant 2",
                                      .rsc = ReservationServiceCode::resy,
                                      .resy_id = 2};
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
        .resy_token = "token",
    };
    booking.create();

    BookingAssertion expected = {
        .user_id = user1.id,
        .timeslot_id = timeslot1.id,
        .resy_token = "token",
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
        .resy_token = "token",
    };
    booking1.create();
    Booking booking2{
        .user_id = user2.id,
        .timeslot_id = timeslot2.id,
    };
    booking2.create();

    std::vector<BookingAssertion> expected = {
        {.user_id = user1.id,
         .timeslot_id = timeslot1.id,
         .resy_token = "token"},
        {.user_id = user2.id, .timeslot_id = timeslot2.id},
    };

    assert_booking_list_in_db(expected);
  }

  SECTION("gets bookings by user id from db") {
    Booking booking1{
        .user_id = user1.id,
        .timeslot_id = timeslot1.id,
        .resy_token = "token",
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
        .resy_token = "token",
    };

    std::vector<Booking> bookings = Booking::get_by_user_id(user1.id);
    assert_booking_list(bookings, {expected});
  }

  SECTION("updates booking with same reference in db") {
    Booking booking{
        .user_id = user1.id,
        .timeslot_id = timeslot1.id,
        .resy_token = "token",
    };

    booking.create();

    booking.resy_token = std::nullopt;
    booking.save();

    BookingAssertion expected = {
        .user_id = user1.id,
        .timeslot_id = timeslot1.id,
    };

    assert_booking_list_in_db({expected});
  }

  SECTION("updates booking with different reference in db") {
    Booking booking{
        .user_id = user1.id,
        .timeslot_id = timeslot1.id,
        .resy_token = "token",
    };

    booking.create();

    std::optional<Booking> other =
        Booking::get(booking.user_id, booking.timeslot_id);
    REQUIRE(other.has_value());

    other.value().resy_token = std::nullopt;
    other.value().save();

    BookingAssertion expected = {
        .user_id = user1.id,
        .timeslot_id = timeslot1.id,
    };

    assert_booking_list_in_db({expected});

    booking.refresh();

    assert_booking(booking, expected);
  }

  SECTION("removes booking from db") {
    Booking booking{
        .user_id = user1.id,
        .timeslot_id = timeslot1.id,
        .resy_token = "token",
    };
    booking.create();

    booking.remove();

    std::vector<BookingAssertion> expected = {};
    assert_booking_list_in_db(expected);
  }

  SECTION("removes booking by user id and timeslot id from db") {
    Booking booking{
        .user_id = user1.id,
        .timeslot_id = timeslot1.id,
        .resy_token = "token",
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
        .resy_token = "token",
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

  SECTION("removes all bookings from db") {
    Booking booking1{
        .user_id = user1.id,
        .timeslot_id = timeslot1.id,
        .resy_token = "token",
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
}