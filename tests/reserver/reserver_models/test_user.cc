#include "src/reserver/reserver_models/user.h"
#include "src/utils/db/db.h"
#include <catch2/catch_test_macros.hpp>
#include <optional>

TEST_CASE("user connects to db correctly",
          "[reserver][reserver_models][user]") {
  User::drop_table();
  User::create_table();

  SECTION("stores and loads user from db") {
    User user{.name = "user 1"};
    user.save();

    std::vector<User> users = User::get_all();
    REQUIRE(users.size() == 1);
    REQUIRE(users[0].id == user.id);
    REQUIRE(users[0].name == "user 1");

    std::optional<User> other = User::get(user.id);
    REQUIRE(other.has_value());
    REQUIRE(other.value().id == user.id);
    REQUIRE(other.value().name == "user 1");
  }

  SECTION("stores and loads multiple users from db") {
    User user1{.name = "user 1"};
    user1.save();
    User user2{.name = "user 2"};
    user2.save();

    std::vector<User> users = User::get_all();
    REQUIRE(users.size() == 2);
    REQUIRE(users[0].name == "user 1");
    REQUIRE(users[1].name == "user 2");
  }

  SECTION("updates user with same reference in db") {
    User user{.name = "user 1"};
    user.save();

    user.name = "user 2";
    user.save();

    std::vector<User> users = User::get_all();
    REQUIRE(users.size() == 1);
    REQUIRE(users[0].id == user.id);
    REQUIRE(users[0].name == "user 2");
  }

  SECTION("updates user with different reference in db") {
    User user{.name = "user 1"};
    user.save();

    std::optional<User> other = User::get(user.id);
    REQUIRE(other.has_value());

    other.value().name = "user 2";
    other.value().save();

    std::vector<User> users = User::get_all();
    REQUIRE(users.size() == 1);
    REQUIRE(users[0].id == user.id);
    REQUIRE(users[0].name == "user 2");

    user.refresh();

    REQUIRE(user.name == "user 2");
  }

  SECTION("removes user from db") {
    User user{.name = "user 1"};
    user.save();

    std::vector<User> users = User::get_all();
    REQUIRE(users.size() == 1);

    user.remove();
    users = User::get_all();
    REQUIRE(users.size() == 0);
  }

  SECTION("removes user by id from db") {
    User user{.name = "user 1"};
    user.save();

    std::vector<User> users = User::get_all();
    REQUIRE(users.size() == 1);

    User::remove(user.id);
    users = User::get_all();
    REQUIRE(users.size() == 0);
  }

  SECTION("removes all users from db") {
    User user1{.name = "user 1"};
    user1.save();
    User user2{.name = "user 2"};
    user2.save();

    std::vector<User> users = User::get_all();
    REQUIRE(users.size() == 2);

    User::remove_all();
    users = User::get_all();
    REQUIRE(users.size() == 0);
  }

  User::drop_table();
}