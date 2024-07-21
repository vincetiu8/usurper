#include "src/reserver/proxies/resy/models/resy_user.h"
#include "src/reserver/reserver_models/user.h"
#include <catch2/catch_test_macros.hpp>
#include <optional>

struct ResyUserAssertion {
  int user_id;
  std::string auth_token;
};

void assert_resy_user(const ResyUser &rsu, const ResyUserAssertion &assertion) {
  REQUIRE(rsu.user_id == assertion.user_id);
  REQUIRE(rsu.auth_token == assertion.auth_token);
}

void assert_resy_user_list(const std::vector<ResyUser> &rsus,
                           const std::vector<ResyUserAssertion> &assertions) {
  REQUIRE(rsus.size() == assertions.size());
  for (size_t i = 0; i < rsus.size(); i++) {
    assert_resy_user(rsus[i], assertions[i]);
  }
}

void assert_resy_user_list_in_db(
    const std::vector<ResyUserAssertion> &assertions) {
  std::vector<ResyUser> rsus = ResyUser::get_all();
  assert_resy_user_list(rsus, assertions);
}

TEST_CASE("resy user interacts with db correctly",
          "[reserver][proxies][resy][resy_user]") {
  ResyUser::drop_table();
  User::drop_table();
  User::create_table();
  ResyUser::create_table();

  User user1{.name = "user 1"};
  user1.save();
  User user2{.name = "user 2"};
  user2.save();

  SECTION("stores and loads resy user from db") {
    ResyUser rsu{
        .user_id = user1.id,
        .auth_token = "auth token 1",
    };
    rsu.save();

    ResyUserAssertion expected = {
        .user_id = user1.id,
        .auth_token = "auth token 1",
    };

    assert_resy_user_list_in_db({expected});

    std::optional<ResyUser> other = ResyUser::get(user1.id);
    REQUIRE(other.has_value());
    assert_resy_user(other.value(), expected);
  }

  SECTION("stores and loads multiple resy users from db") {
    ResyUser rsu1{
        .user_id = user1.id,
        .auth_token = "auth token 1",
    };
    rsu1.save();
    ResyUser rsu2{.user_id = user2.id, .auth_token = "auth token 2"};
    rsu2.save();

    std::vector<ResyUserAssertion> expected = {
        {.user_id = user1.id, .auth_token = "auth token 1"},
        {.user_id = user2.id, .auth_token = "auth token 2"},
    };

    assert_resy_user_list_in_db(expected);
  }

  SECTION("updates resy user with same reference in db") {
    ResyUser rsu{
        .user_id = user1.id,
        .auth_token = "auth token 1",
    };
    rsu.save();

    rsu.auth_token = "auth token 2";
    rsu.save();

    ResyUserAssertion expected = {
        .user_id = user1.id,
        .auth_token = "auth token 2",
    };

    assert_resy_user_list_in_db({expected});
  }

  SECTION("updates resy user with different reference in db") {
    ResyUser rsu{.user_id = user1.id, .auth_token = "auth token 1"};
    rsu.save();

    std::optional<ResyUser> other = ResyUser::get(user1.id);
    REQUIRE(other.has_value());

    other.value().auth_token = "auth token 2";
    other.value().save();

    ResyUserAssertion expected = {
        .user_id = user1.id,
        .auth_token = "auth token 2",
    };

    assert_resy_user_list_in_db({expected});

    rsu.refresh();

    assert_resy_user(rsu, expected);
  }

  SECTION("removes resy user from db") {
    ResyUser rsu{
        .user_id = user1.id,
        .auth_token = "auth token 1",
    };
    rsu.save();

    rsu.remove();

    assert_resy_user_list_in_db({});
  }

  SECTION("removes resy user by user_id from db") {
    ResyUser rsu1{.user_id = user1.id, .auth_token = "auth token 1"};
    rsu1.save();
    ResyUser rsu2{
        .user_id = user2.id,
        .auth_token = "auth token 2",
    };
    rsu2.save();

    ResyUser::remove(user1.id);

    ResyUserAssertion expected = {
        .user_id = user2.id,
        .auth_token = "auth token 2",
    };

    assert_resy_user_list_in_db({expected});
  }

  SECTION("removes all resy users from db") {
    ResyUser rsu1{.user_id = user1.id, .auth_token = "auth token 1"};
    rsu1.save();
    ResyUser rsu2{.user_id = user2.id, .auth_token = "auth token 2"};
    rsu2.save();

    ResyUser::remove_all();

    assert_resy_user_list_in_db({});
  }
}
