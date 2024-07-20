#include "src/reserver/reserver_models/reservation_service_user.h"
#include "src/reserver/reserver_models/user.h"
#include "src/utils/db/db.h"
#include <catch2/catch_test_macros.hpp>
#include <optional>

TEST_CASE("reservation service user connects to db correctly",
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
        .reservation_service_code = ReservationServiceCode::resy,
        .user_id = user1.id,
        .auth_token = "auth token 1",
    };
    rsu.save();

    std::vector<ReservationServiceUser> rsus =
        ReservationServiceUser::get_all();
    REQUIRE(rsus.size() == 1);
    REQUIRE(rsus[0].reservation_service_code == ReservationServiceCode::resy);
    REQUIRE(rsus[0].user_id == user1.id);
    REQUIRE(rsus[0].auth_token == "auth token 1");

    std::optional<ReservationServiceUser> other =
        ReservationServiceUser::get(ReservationServiceCode::resy, user1.id);
    REQUIRE(other.has_value());
    REQUIRE(other.value().reservation_service_code ==
            ReservationServiceCode::resy);
    REQUIRE(other.value().user_id == user1.id);
    REQUIRE(other.value().auth_token == "auth token 1");
  }

  SECTION("stores and loads multiple reservation service users from db") {
    ReservationServiceUser rsu1{
        .reservation_service_code = ReservationServiceCode::resy,
        .user_id = user1.id,
        .auth_token = "auth token 1",
    };
    rsu1.save();
    ReservationServiceUser rsu2{.reservation_service_code =
                                    ReservationServiceCode::resy,
                                .user_id = user2.id,
                                .auth_token = "auth token 2"};
    rsu2.save();

    std::vector<ReservationServiceUser> rsus =
        ReservationServiceUser::get_all();
    REQUIRE(rsus.size() == 2);
    REQUIRE(rsus[0].auth_token == "auth token 1");
    REQUIRE(rsus[1].auth_token == "auth token 2");
  }

  SECTION("updates reservation service user with same reference in db") {
    ReservationServiceUser rsu{
        .reservation_service_code = ReservationServiceCode::resy,
        .user_id = user1.id,
        .auth_token = "auth token 1",
    };
    rsu.save();

    rsu.auth_token = "auth token 2";
    rsu.save();

    std::vector<ReservationServiceUser> rsus =
        ReservationServiceUser::get_all();
    REQUIRE(rsus.size() == 1);
    REQUIRE(rsus[0].reservation_service_code == ReservationServiceCode::resy);
    REQUIRE(rsus[0].user_id == rsu.user_id);
    REQUIRE(rsus[0].auth_token == "auth token 2");
  }

  SECTION("updates reservation service user with different reference in db") {
    ReservationServiceUser rsu{.reservation_service_code =
                                   ReservationServiceCode::resy,
                               .user_id = user1.id,
                               .auth_token = "auth token 1"};
    rsu.save();

    std::optional<ReservationServiceUser> other =
        ReservationServiceUser::get(ReservationServiceCode::resy, 1);
    REQUIRE(other.has_value());

    other.value().auth_token = "auth token 2";
    other.value().save();

    std::vector<ReservationServiceUser> rsus =
        ReservationServiceUser::get_all();
    REQUIRE(rsus.size() == 1);
    REQUIRE(rsus[0].reservation_service_code == ReservationServiceCode::resy);
    REQUIRE(rsus[0].user_id == rsu.user_id);
    REQUIRE(rsus[0].auth_token == "auth token 2");
  }

  SECTION("removes reservation service user from db") {
    ReservationServiceUser rsu{
        .reservation_service_code = ReservationServiceCode::resy,
        .user_id = user1.id,
        .auth_token = "auth token 1",
    };
    rsu.save();

    std::vector<ReservationServiceUser> rsus =
        ReservationServiceUser::get_all();
    REQUIRE(rsus.size() == 1);

    rsu.remove();
    rsus = ReservationServiceUser::get_all();
    REQUIRE(rsus.size() == 0);
  }

  SECTION("removes reservation service user by reservation service code and "
          "user_id from db") {
    ReservationServiceUser rsu1{.reservation_service_code =
                                    ReservationServiceCode::resy,
                                .user_id = user1.id,
                                .auth_token = "auth token 1"};
    rsu1.save();
    ReservationServiceUser rsu2(ReservationServiceCode::resy, user2.id,
                                "auth token 2");
    rsu2.save();

    std::vector<ReservationServiceUser> rsus =
        ReservationServiceUser::get_all();
    REQUIRE(rsus.size() == 2);

    ReservationServiceUser::remove(ReservationServiceCode::resy, user1.id);
    rsus = ReservationServiceUser::get_all();
    REQUIRE(rsus.size() == 1);
    REQUIRE(rsus[0].reservation_service_code == ReservationServiceCode::resy);
    REQUIRE(rsus[0].user_id == user2.id);
  }

  SECTION("removes reservation service user by user_id from db") {
    ReservationServiceUser rsu1{.reservation_service_code =
                                    ReservationServiceCode::resy,
                                .user_id = user1.id,
                                .auth_token = "auth token 1"};
    rsu1.save();
    ReservationServiceUser rsu2{.reservation_service_code =
                                    ReservationServiceCode::resy,
                                .user_id = user2.id,
                                .auth_token = "auth token 2"};
    rsu2.save();

    std::vector<ReservationServiceUser> rsus =
        ReservationServiceUser::get_all();
    REQUIRE(rsus.size() == 2);

    ReservationServiceUser::remove_by_user_id(user1.id);
    rsus = ReservationServiceUser::get_all();
    REQUIRE(rsus.size() == 1);
  }

  SECTION("removes all reservation service users from db") {
    ReservationServiceUser rsu1{.reservation_service_code =
                                    ReservationServiceCode::resy,
                                .user_id = user1.id,
                                .auth_token = "auth token 1"};
    rsu1.save();
    ReservationServiceUser rsu2{.reservation_service_code =
                                    ReservationServiceCode::resy,
                                .user_id = user2.id,
                                .auth_token = "auth token 2"};
    rsu2.save();

    std::vector<ReservationServiceUser> rsus =
        ReservationServiceUser::get_all();
    REQUIRE(rsus.size() == 2);

    ReservationServiceUser::remove_all();
    rsus = ReservationServiceUser::get_all();
    REQUIRE(rsus.size() == 0);
  }

  ReservationServiceUser::drop_table();
  User::drop_table();
}
