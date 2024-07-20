#include "src/reserver/reserver_models/restaurant.h"
#include <catch2/catch_test_macros.hpp>
#include <optional>

TEST_CASE("restaurant connects to db correctly",
          "[reserver][reserver_models][restaurant]") {
  Restaurant::drop_table();
  Restaurant::create_table();

  SECTION("stores and loads restaurant from db") {
    Restaurant restaurant{.name = "restaurant 1"};
    restaurant.save();

    std::vector<Restaurant> restaurants = Restaurant::get_all();
    REQUIRE(restaurants.size() == 1);
    REQUIRE(restaurants[0].name == "restaurant 1");

    std::optional<Restaurant> other = Restaurant::get(restaurants[0].id);
    REQUIRE(other.has_value());
    REQUIRE(other.value().name == "restaurant 1");
  }

  SECTION("stores and loads multiple restaurants from db") {
    Restaurant restaurant1{.name = "restaurant 1"};
    restaurant1.save();
    Restaurant restaurant2{.name = "restaurant 2"};
    restaurant2.save();

    std::vector<Restaurant> restaurants = Restaurant::get_all();
    REQUIRE(restaurants.size() == 2);
    REQUIRE(restaurants[0].name == "restaurant 1");
    REQUIRE(restaurants[1].name == "restaurant 2");
  }

  SECTION("gets restaurants by name from db") {
    Restaurant restaurant1{.name = "restaurant 1"};
    restaurant1.save();
    Restaurant restaurant2{.name = "restaurant 2"};
    restaurant2.save();

    std::string name = "restaurant 1";
    std::vector<Restaurant> restaurants = Restaurant::get_by_name(name);
    REQUIRE(restaurants.size() == 1);
    REQUIRE(restaurants[0].name == "restaurant 1");

    name = "restaurant";
    restaurants = Restaurant::get_by_name(name);
    REQUIRE(restaurants.size() == 2);
    REQUIRE(restaurants[0].name == "restaurant 1");
    REQUIRE(restaurants[1].name == "restaurant 2");

    name = "res";
    restaurants = Restaurant::get_by_name(name);
    REQUIRE(restaurants.size() == 2);
    REQUIRE(restaurants[0].name == "restaurant 1");
    REQUIRE(restaurants[1].name == "restaurant 2");

    name = "rant";
    restaurants = Restaurant::get_by_name(name);
    REQUIRE(restaurants.size() == 2);
    REQUIRE(restaurants[0].name == "restaurant 1");
    REQUIRE(restaurants[1].name == "restaurant 2");

    name = "restaurant 3";
    restaurants = Restaurant::get_by_name(name);
    REQUIRE(restaurants.size() == 0);
  }

  SECTION("updates restaurant with same reference in db") {
    Restaurant restaurant{.name = "restaurant 1"};
    restaurant.save();

    restaurant.name = "restaurant 2";
    restaurant.save();

    std::vector<Restaurant> restaurants = Restaurant::get_all();
    REQUIRE(restaurants.size() == 1);
    REQUIRE(restaurants[0].id == restaurant.id);
    REQUIRE(restaurants[0].name == "restaurant 2");
  }

  SECTION("updates restaurant with different reference in db") {
    Restaurant restaurant{.name = "restaurant 1"};
    restaurant.save();

    std::optional<Restaurant> other = Restaurant::get(restaurant.id);
    REQUIRE(other.has_value());

    other.value().name = "restaurant 2";
    other.value().save();

    std::vector<Restaurant> restaurants = Restaurant::get_all();
    REQUIRE(restaurants.size() == 1);
    REQUIRE(restaurants[0].id == restaurant.id);
    REQUIRE(restaurants[0].name == "restaurant 2");

    restaurant.refresh();

    REQUIRE(restaurant.name == "restaurant 2");
  }

  SECTION("removes restaurant from db") {
    Restaurant restaurant{.name = "restaurant 1"};
    restaurant.save();

    Restaurant::remove(restaurant.id);

    std::vector<Restaurant> restaurants = Restaurant::get_all();
    REQUIRE(restaurants.size() == 0);
  }

  SECTION("removes all restaurants from db") {
    Restaurant restaurant1{.name = "restaurant 1"};
    restaurant1.save();
    Restaurant restaurant2{.name = "restaurant 2"};
    restaurant2.save();

    Restaurant::remove_all();

    std::vector<Restaurant> restaurants = Restaurant::get_all();
    REQUIRE(restaurants.size() == 0);
  }
}
