#include "src/reserver/reserver_models/restaurant.h"
#include <catch2/catch_test_macros.hpp>
#include <optional>

struct RestaurantAssertion {
  int id;
  std::string name;
};

void assert_restaurant(const Restaurant &restaurant,
                       const RestaurantAssertion &assertion) {
  REQUIRE(restaurant.id == assertion.id);
  REQUIRE(restaurant.name == assertion.name);
}

void assert_restaurant_list(
    const std::vector<Restaurant> &restaurants,
    const std::vector<RestaurantAssertion> &assertions) {
  REQUIRE(restaurants.size() == assertions.size());
  for (size_t i = 0; i < restaurants.size(); i++) {
    assert_restaurant(restaurants[i], assertions[i]);
  }
}

void assert_restaurant_list_in_db(
    const std::vector<RestaurantAssertion> &assertions) {
  std::vector<Restaurant> restaurants = Restaurant::get_all();
  assert_restaurant_list(restaurants, assertions);
}

TEST_CASE("restaurant interacts with db correctly",
          "[reserver][reserver_models][restaurant]") {
  Restaurant::drop_table();
  Restaurant::create_table();

  SECTION("stores and loads restaurant from db") {
    Restaurant restaurant{.name = "restaurant 1"};
    restaurant.save();

    RestaurantAssertion expected = {
        .id = restaurant.id,
        .name = "restaurant 1",
    };

    std::optional<Restaurant> other = Restaurant::get(restaurant.id);
    REQUIRE(other.has_value());
    assert_restaurant(other.value(), expected);
  }

  SECTION("stores and loads multiple restaurants from db") {
    Restaurant restaurant1{.name = "restaurant 1"};
    restaurant1.save();
    Restaurant restaurant2{.name = "restaurant 2"};
    restaurant2.save();

    std::vector<RestaurantAssertion> expected = {
        {.id = restaurant1.id, .name = "restaurant 1"},
        {.id = restaurant2.id, .name = "restaurant 2"},
    };

    assert_restaurant_list_in_db(expected);
  }

  SECTION("gets restaurants by name from db") {
    Restaurant restaurant1{.name = "restaurant 1"};
    restaurant1.save();
    Restaurant restaurant2{.name = "restaurant 2"};
    restaurant2.save();

    RestaurantAssertion expected1 = {
        .id = restaurant1.id,
        .name = "restaurant 1",
    };
    RestaurantAssertion expected2 = {
        .id = restaurant2.id,
        .name = "restaurant 2",
    };

    std::string name = "restaurant 1";
    std::vector<Restaurant> restaurants = Restaurant::get_by_name(name);
    assert_restaurant_list(restaurants, {expected1});

    name = "restaurant";
    restaurants = Restaurant::get_by_name(name);
    assert_restaurant_list(restaurants, {expected1, expected2});

    name = "res";
    restaurants = Restaurant::get_by_name(name);
    assert_restaurant_list(restaurants, {expected1, expected2});

    name = "rant";
    restaurants = Restaurant::get_by_name(name);
    assert_restaurant_list(restaurants, {expected1, expected2});

    name = "restaurant 3";
    restaurants = Restaurant::get_by_name(name);
    assert_restaurant_list(restaurants, {});
  }

  SECTION("updates restaurant with same reference in db") {
    Restaurant restaurant{.name = "restaurant 1"};
    restaurant.save();

    restaurant.name = "restaurant 2";
    restaurant.save();

    RestaurantAssertion expected = {
        .id = restaurant.id,
        .name = "restaurant 2",
    };

    assert_restaurant_list_in_db({expected});
  }

  SECTION("updates restaurant with different reference in db") {
    Restaurant restaurant{.name = "restaurant 1"};
    restaurant.save();

    std::optional<Restaurant> other = Restaurant::get(restaurant.id);
    REQUIRE(other.has_value());

    other.value().name = "restaurant 2";
    other.value().save();

    RestaurantAssertion expected = {
        .id = restaurant.id,
        .name = "restaurant 2",
    };

    assert_restaurant_list_in_db({expected});

    restaurant.refresh();

    assert_restaurant(restaurant, expected);
  }

  SECTION("removes restaurant from db") {
    Restaurant restaurant{.name = "restaurant 1"};
    restaurant.save();

    restaurant.remove();

    assert_restaurant_list_in_db({});
  }

  SECTION("removes restaurant by id from db") {
    Restaurant restaurant{.name = "restaurant 1"};
    restaurant.save();

    Restaurant::remove(restaurant.id);

    assert_restaurant_list_in_db({});
  }

  SECTION("removes all restaurants from db") {
    Restaurant restaurant1{.name = "restaurant 1"};
    restaurant1.save();
    Restaurant restaurant2{.name = "restaurant 2"};
    restaurant2.save();

    Restaurant::remove_all();

    assert_restaurant_list_in_db({});
  }
}
