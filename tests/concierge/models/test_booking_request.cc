#include "src/concierge/models/booking_request.h"
#include "src/reserver/models/restaurant.h"
#include "src/reserver/models/user.h"
#include <catch2/catch_test_macros.hpp>
#include <optional>

struct BookingRequestAssertion {
  int id;
  int user_id;
  int restaurant_id;
  Date date;
  Time earliest_time;
  Time latest_time;
  int party_size;
};

void assert_BookingRequest_request(const BookingRequest &request,
                                   const BookingRequestAssertion &assertion) {
  REQUIRE(request.id == assertion.id);
  REQUIRE(request.user_id == assertion.user_id);
  REQUIRE(request.restaurant_id == assertion.restaurant_id);
  REQUIRE(request.date == assertion.date);
  REQUIRE(request.earliest_time == assertion.earliest_time);
  REQUIRE(request.latest_time == assertion.latest_time);
  REQUIRE(request.party_size == assertion.party_size);
}

void assert_BookingRequest_request_list(
    const std::vector<BookingRequest> &requests,
    const std::vector<BookingRequestAssertion> &assertions) {
  REQUIRE(requests.size() == assertions.size());
  for (size_t i = 0; i < requests.size(); i++) {
    assert_BookingRequest_request(requests[i], assertions[i]);
  }
}

void assert_BookingRequest_request_list_in_db(
    const std::vector<BookingRequestAssertion> &assertions) {
  std::vector<BookingRequest> requests = BookingRequest::get_all();
  assert_BookingRequest_request_list(requests, assertions);
}

TEST_CASE("BookingRequest request interacts with db correctly",
          "[concierge][models][BookingRequest_request]") {
  BookingRequest::drop_table();
  Restaurant::drop_table();
  User::drop_table();
  User::create_table();
  Restaurant::create_table();
  BookingRequest::create_table();

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

  SECTION("stores and loads booking request from db") {
    BookingRequest request{
        .user_id = user1.id,
        .restaurant_id = restaurant1.id,
        .date = date1,
        .earliest_time = start_time1,
        .latest_time = end_time1,
        .party_size = 2,
    };
    request.create();

    BookingRequestAssertion expected = {
        .id = request.id,
        .user_id = user1.id,
        .restaurant_id = restaurant1.id,
        .date = date1,
        .earliest_time = start_time1,
        .latest_time = end_time1,
        .party_size = 2,
    };

    std::optional<BookingRequest> other = BookingRequest::get(request.id);
    REQUIRE(other.has_value());
    assert_BookingRequest_request(other.value(), expected);
  }

  SECTION("stores and loads multiple booking requests from db") {
    BookingRequest request1{
        .user_id = user1.id,
        .restaurant_id = restaurant1.id,
        .date = date1,
        .earliest_time = start_time1,
        .latest_time = end_time1,
        .party_size = 2,
    };
    request1.create();
    BookingRequest request2{
        .user_id = user2.id,
        .restaurant_id = restaurant2.id,
        .date = date2,
        .earliest_time = start_time2,
        .latest_time = end_time2,
        .party_size = 4,
    };
    request2.create();

    std::vector<BookingRequestAssertion> expected = {
        {
            .id = request1.id,
            .user_id = user1.id,
            .restaurant_id = restaurant1.id,
            .date = date1,
            .earliest_time = start_time1,
            .latest_time = end_time1,
            .party_size = 2,
        },
        {
            .id = request2.id,
            .user_id = user2.id,
            .restaurant_id = restaurant2.id,
            .date = date2,
            .earliest_time = start_time2,
            .latest_time = end_time2,
            .party_size = 4,
        },
    };

    assert_BookingRequest_request_list_in_db(expected);
  }

  SECTION("gets booking request by user id from db") {
    BookingRequest request1{
        .user_id = user1.id,
        .restaurant_id = restaurant1.id,
        .date = date1,
        .earliest_time = start_time1,
        .latest_time = end_time1,
        .party_size = 2,
    };
    request1.create();
    BookingRequest request2{
        .user_id = user2.id,
        .restaurant_id = restaurant2.id,
        .date = date2,
        .earliest_time = start_time2,
        .latest_time = end_time2,
        .party_size = 4,
    };
    request2.create();

    BookingRequestAssertion expected = {
        .id = request1.id,
        .user_id = user1.id,
        .restaurant_id = restaurant1.id,
        .date = date1,
        .earliest_time = start_time1,
        .latest_time = end_time1,
        .party_size = 2,
    };

    std::vector<BookingRequest> BookingRequests =
        BookingRequest::get_by_user_id(user1.id);
    assert_BookingRequest_request_list(BookingRequests, {expected});
  }

  SECTION("updates booking request with same reference in db") {
    BookingRequest request{
        .user_id = user1.id,
        .restaurant_id = restaurant1.id,
        .date = date1,
        .earliest_time = start_time1,
        .latest_time = end_time1,
        .party_size = 2,
    };

    request.create();

    request.date = date2;
    request.earliest_time = start_time2;
    request.latest_time = end_time2;
    request.party_size = 4;
    request.save();

    BookingRequestAssertion expected = {
        .id = request.id,
        .user_id = user1.id,
        .restaurant_id = restaurant1.id,
        .date = date2,
        .earliest_time = start_time2,
        .latest_time = end_time2,
        .party_size = 4,
    };

    assert_BookingRequest_request_list_in_db({expected});
  }

  SECTION("updates booking request with different reference in db") {
    BookingRequest request{
        .user_id = user1.id,
        .restaurant_id = restaurant1.id,
        .date = date1,
        .earliest_time = start_time1,
        .latest_time = end_time1,
        .party_size = 2,
    };

    request.create();

    std::optional<BookingRequest> other = BookingRequest::get(request.id);
    REQUIRE(other.has_value());

    other.value().date = date2;
    other.value().earliest_time = start_time2;
    other.value().latest_time = end_time2;
    other.value().party_size = 4;
    other.value().save();

    BookingRequestAssertion expected = {
        .id = request.id,
        .user_id = user1.id,
        .restaurant_id = restaurant1.id,
        .date = date2,
        .earliest_time = start_time2,
        .latest_time = end_time2,
        .party_size = 4,
    };

    assert_BookingRequest_request_list_in_db({expected});

    request.refresh();

    assert_BookingRequest_request(request, expected);
  }

  SECTION("removes booking request from db") {
    BookingRequest BookingRequest{
        .user_id = user1.id,
        .restaurant_id = restaurant1.id,
        .date = date1,
        .earliest_time = start_time1,
        .latest_time = end_time1,
        .party_size = 2,
    };
    BookingRequest.create();

    BookingRequest.remove();

    std::vector<BookingRequestAssertion> expected = {};
    assert_BookingRequest_request_list_in_db(expected);
  }

  SECTION("removes booking request by id from db") {
    BookingRequest request{
        .user_id = user1.id,
        .restaurant_id = restaurant1.id,
        .date = date1,
        .earliest_time = start_time1,
        .latest_time = end_time1,
        .party_size = 2,
    };
    request.create();

    BookingRequest::remove(request.id);

    std::vector<BookingRequestAssertion> expected = {};
    assert_BookingRequest_request_list_in_db(expected);
  }

  SECTION("deletes BookingRequest by user id from db") {
    BookingRequest request1{
        .user_id = user1.id,
        .restaurant_id = restaurant1.id,
        .date = date1,
        .earliest_time = start_time1,
        .latest_time = end_time1,
        .party_size = 2,
    };
    request1.create();
    BookingRequest request2{
        .user_id = user1.id,
        .restaurant_id = restaurant2.id,
        .date = date2,
        .earliest_time = start_time2,
        .latest_time = end_time2,
        .party_size = 4,
    };
    request2.create();

    BookingRequest::remove_by_user_id(user1.id);

    std::vector<BookingRequestAssertion> expected = {};
    assert_BookingRequest_request_list_in_db(expected);
  }

  SECTION("removes all booking requests from db") {
    BookingRequest request1{
        .user_id = user1.id,
        .restaurant_id = restaurant1.id,
        .date = date1,
        .earliest_time = start_time1,
        .latest_time = end_time1,
        .party_size = 2,
    };
    request1.create();
    BookingRequest request2{
        .user_id = user2.id,
        .restaurant_id = restaurant2.id,
        .date = date2,
        .earliest_time = start_time2,
        .latest_time = end_time2,
        .party_size = 4,
    };
    request2.create();

    BookingRequest::remove_all();

    std::vector<BookingRequestAssertion> expected = {};
    assert_BookingRequest_request_list_in_db(expected);
  }
}