#include "src/reserver/reserver_models/reservation_service_user.h"
#include "src/reserver/reserver_models/user.h"
#include <catch2/catch_test_macros.hpp>
#include <optional>

struct ReservationServiceUserAssertion {
  int user_id;
  ReservationServiceCode reservation_service_code;
  std::string auth_token;
};

void assert_reservation_service_user(
    const ReservationServiceUser &rsu,
    const ReservationServiceUserAssertion &assertion) {
  REQUIRE(rsu.user_id == assertion.user_id);
  REQUIRE(rsu.reservation_service_code == assertion.reservation_service_code);
  REQUIRE(rsu.auth_token == assertion.auth_token);
}

void assert_reservation_service_user_list(
    const std::vector<ReservationServiceUser> &rsus,
    const std::vector<ReservationServiceUserAssertion> &assertions) {
  REQUIRE(rsus.size() == assertions.size());
  for (size_t i = 0; i < rsus.size(); i++) {
    assert_reservation_service_user(rsus[i], assertions[i]);
  }
}

void assert_reservation_service_user_list_in_db(
    const std::vector<ReservationServiceUserAssertion> &assertions) {
  std::vector<ReservationServiceUser> rsus = ReservationServiceUser::get_all();
  assert_reservation_service_user_list(rsus, assertions);
}

TEST_CASE("reservation service user interacts with db correctly",
          "[reserver][reserver_models][reservation_service_user]") {
  ReservationServiceUser::drop_table();
  User::drop_table();
  User::create_table();
  ReservationServiceUser::create_table();

  User user1{.name = "user 1"};
  user1.save();
  User user2{.name = "user 2"};
  user2.save();

  SECTION("stores and loads reservation service user from db") {
    ReservationServiceUser rsu{
        .user_id = user1.id,
        .reservation_service_code = ReservationServiceCode::resy,

        .auth_token = "auth token 1",
    };
    rsu.save();

    ReservationServiceUserAssertion expected = {
        .user_id = user1.id,
        .reservation_service_code = ReservationServiceCode::resy,
        .auth_token = "auth token 1",
    };

    assert_reservation_service_user_list_in_db({expected});

    std::optional<ReservationServiceUser> other =
        ReservationServiceUser::get(ReservationServiceCode::resy, user1.id);
    REQUIRE(other.has_value());
    assert_reservation_service_user(other.value(), expected);
  }

  SECTION("stores and loads multiple reservation service users from db") {
    ReservationServiceUser rsu1{
        .user_id = user1.id,
        .reservation_service_code = ReservationServiceCode::resy,

        .auth_token = "auth token 1",
    };
    rsu1.save();
    ReservationServiceUser rsu2{.user_id = user2.id,
                                .reservation_service_code =
                                    ReservationServiceCode::resy,

                                .auth_token = "auth token 2"};
    rsu2.save();

    std::vector<ReservationServiceUserAssertion> expected = {
        {.user_id = user1.id,
         .reservation_service_code = ReservationServiceCode::resy,
         .auth_token = "auth token 1"},
        {.user_id = user2.id,
         .reservation_service_code = ReservationServiceCode::resy,
         .auth_token = "auth token 2"},
    };

    assert_reservation_service_user_list_in_db(expected);
  }

  SECTION("updates reservation service user with same reference in db") {
    ReservationServiceUser rsu{
        .user_id = user1.id,
        .reservation_service_code = ReservationServiceCode::resy,

        .auth_token = "auth token 1",
    };
    rsu.save();

    rsu.auth_token = "auth token 2";
    rsu.save();

    ReservationServiceUserAssertion expected = {
        .user_id = user1.id,
        .reservation_service_code = ReservationServiceCode::resy,
        .auth_token = "auth token 2",
    };

    assert_reservation_service_user_list_in_db({expected});
  }

  SECTION("updates reservation service user with different reference in db") {
    ReservationServiceUser rsu{.user_id = user1.id,
                               .reservation_service_code =
                                   ReservationServiceCode::resy,

                               .auth_token = "auth token 1"};
    rsu.save();

    std::optional<ReservationServiceUser> other =
        ReservationServiceUser::get(ReservationServiceCode::resy, 1);
    REQUIRE(other.has_value());

    other.value().auth_token = "auth token 2";
    other.value().save();

    ReservationServiceUserAssertion expected = {
        .user_id = user1.id,
        .reservation_service_code = ReservationServiceCode::resy,
        .auth_token = "auth token 2",
    };

    assert_reservation_service_user_list_in_db({expected});

    rsu.refresh();

    assert_reservation_service_user(rsu, expected);
  }

  SECTION("removes reservation service user from db") {
    ReservationServiceUser rsu{
        .user_id = user1.id,
        .reservation_service_code = ReservationServiceCode::resy,

        .auth_token = "auth token 1",
    };
    rsu.save();

    rsu.remove();

    assert_reservation_service_user_list_in_db({});
  }

  SECTION("removes reservation service user by reservation service code and "
          "user_id from db") {
    ReservationServiceUser rsu1{.user_id = user1.id,
                                .reservation_service_code =
                                    ReservationServiceCode::resy,

                                .auth_token = "auth token 1"};
    rsu1.save();
    ReservationServiceUser rsu2{
        .user_id = user2.id,
        .reservation_service_code = ReservationServiceCode::resy,
        .auth_token = "auth token 2",
    };
    rsu2.save();

    ReservationServiceUser::remove(ReservationServiceCode::resy, user1.id);

    ReservationServiceUserAssertion expected = {
        .user_id = user2.id,
        .reservation_service_code = ReservationServiceCode::resy,
        .auth_token = "auth token 2",
    };

    assert_reservation_service_user_list_in_db({expected});
  }

  SECTION("removes reservation service user by user_id from db") {
    ReservationServiceUser rsu1{.user_id = user1.id,
                                .reservation_service_code =
                                    ReservationServiceCode::resy,

                                .auth_token = "auth token 1"};
    rsu1.save();
    ReservationServiceUser rsu2{.user_id = user2.id,
                                .reservation_service_code =
                                    ReservationServiceCode::resy,

                                .auth_token = "auth token 2"};
    rsu2.save();

    ReservationServiceUser::remove_by_user_id(user1.id);

    ReservationServiceUserAssertion expected = {
        .user_id = user2.id,
        .reservation_service_code = ReservationServiceCode::resy,
        .auth_token = "auth token 2",
    };

    assert_reservation_service_user_list_in_db({expected});
  }

  SECTION("removes all reservation service users from db") {
    ReservationServiceUser rsu1{.user_id = user1.id,
                                .reservation_service_code =
                                    ReservationServiceCode::resy,
                                .auth_token = "auth token 1"};
    rsu1.save();
    ReservationServiceUser rsu2{

        .user_id = user2.id,
        .reservation_service_code = ReservationServiceCode::resy,
        .auth_token = "auth token 2"};
    rsu2.save();

    ReservationServiceUser::remove_all();

    assert_reservation_service_user_list_in_db({});
  }

  ReservationServiceUser::drop_table();
  User::drop_table();
}
