#include "src/reserver/reserver_models/user.h"
#include <catch2/catch_test_macros.hpp>
#include <optional>

struct UserAssertion {
  int id;
  std::string name;
};

void assert_user(const User &user, const UserAssertion &assertion) {
  REQUIRE(user.id == assertion.id);
  REQUIRE(user.name == assertion.name);
}

void assert_user_list(const std::vector<User> &users,
                      const std::vector<UserAssertion> &assertions) {
  REQUIRE(users.size() == assertions.size());
  for (size_t i = 0; i < users.size(); i++) {
    assert_user(users[i], assertions[i]);
  }
}

void assert_user_list_in_db(const std::vector<UserAssertion> &assertions) {
  std::vector<User> users = User::get_all();
  assert_user_list(users, assertions);
}

TEST_CASE("user interacts with db correctly",
          "[reserver][reserver_models][user]") {
  User::drop_table();
  User::create_table();

  SECTION("stores and loads user from db") {
    User user{.name = "user 1"};
    user.save();

    UserAssertion expected = {
        .id = user.id,
        .name = "user 1",
    };

    assert_user_list_in_db({expected});

    std::optional<User> other = User::get(user.id);
    REQUIRE(other.has_value());
    assert_user(other.value(), expected);
  }

  SECTION("stores and loads multiple users from db") {
    User user1{.name = "user 1"};
    user1.save();
    User user2{.name = "user 2"};
    user2.save();

    std::vector<UserAssertion> expected = {
        {.id = user1.id, .name = "user 1"},
        {.id = user2.id, .name = "user 2"},
    };

    assert_user_list_in_db(expected);
  }

  SECTION("updates user with same reference in db") {
    User user{.name = "user 1"};
    user.save();

    user.name = "user 2";
    user.save();

    UserAssertion expected = {
        .id = user.id,
        .name = "user 2",
    };

    assert_user_list_in_db({expected});
  }

  SECTION("updates user with different reference in db") {
    User user{.name = "user 1"};
    user.save();

    std::optional<User> other = User::get(user.id);
    REQUIRE(other.has_value());

    other.value().name = "user 2";
    other.value().save();

    UserAssertion expected = {
        .id = user.id,
        .name = "user 2",
    };

    assert_user_list_in_db({expected});

    user.refresh();

    assert_user(user, expected);
  }

  SECTION("removes user from db") {
    User user{.name = "user 1"};
    user.save();

    user.remove();
    assert_user_list_in_db({});
  }

  SECTION("removes user by id from db") {
    User user{.name = "user 1"};
    user.save();

    User::remove(user.id);
    assert_user_list_in_db({});
  }

  SECTION("removes all users from db") {
    User user1{.name = "user 1"};
    user1.save();
    User user2{.name = "user 2"};
    user2.save();

    User::remove_all();
    assert_user_list_in_db({});
  }

  User::drop_table();
}