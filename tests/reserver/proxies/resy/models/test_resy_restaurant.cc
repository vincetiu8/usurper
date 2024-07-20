#include "src/reserver/proxies/resy/models/resy_restaurant.h"
#include "src/reserver/reserver_models/restaurant.h"
#include <catch2/catch_test_macros.hpp>
#include <optional>

struct ResyRestaurantAssertion {
  int restaurant_id;
  int resy_id;
};

void assert_resy_restaurant(const ResyRestaurant &rsu,
                            const ResyRestaurantAssertion &assertion) {
  REQUIRE(rsu.restaurant_id == assertion.restaurant_id);
  REQUIRE(rsu.resy_id == assertion.resy_id);
}

void assert_resy_restaurant_list(
    const std::vector<ResyRestaurant> &rsus,
    const std::vector<ResyRestaurantAssertion> &assertions) {
  REQUIRE(rsus.size() == assertions.size());
  for (size_t i = 0; i < rsus.size(); i++) {
    assert_resy_restaurant(rsus[i], assertions[i]);
  }
}

void assert_resy_restaurant_list_in_db(
    const std::vector<ResyRestaurantAssertion> &assertions) {
  std::vector<ResyRestaurant> rsus = ResyRestaurant::get_all();
  assert_resy_restaurant_list(rsus, assertions);
}

TEST_CASE("resy restaurant interacts with db correctly",
          "[reserver][proxies][resy][resy_restaurant]") {
  ResyRestaurant::drop_table();
  Restaurant::drop_table();
  Restaurant::create_table();
  ResyRestaurant::create_table();

  Restaurant restaurant1{.name = "restaurant 1"};
  restaurant1.save();
  Restaurant restaurant2{.name = "restaurant 2"};
  restaurant2.save();

  SECTION("stores and loads resy restaurant from db") {
    ResyRestaurant rsu{
        .restaurant_id = restaurant1.id,
        .resy_id = 1,
    };
    rsu.save();

    ResyRestaurantAssertion expected = {
        .restaurant_id = restaurant1.id,
        .resy_id = 1,
    };

    assert_resy_restaurant_list_in_db({expected});

    std::optional<ResyRestaurant> other = ResyRestaurant::get(restaurant1.id);
    REQUIRE(other.has_value());
    assert_resy_restaurant(other.value(), expected);
  }

  SECTION("stores and loads multiple resy restaurants from db") {
    ResyRestaurant rsu1{
        .restaurant_id = restaurant1.id,
        .resy_id = 1,
    };
    rsu1.save();
    ResyRestaurant rsu2{.restaurant_id = restaurant2.id, .resy_id = 2};
    rsu2.save();

    std::vector<ResyRestaurantAssertion> expected = {
        {.restaurant_id = restaurant1.id, .resy_id = 1},
        {.restaurant_id = restaurant2.id, .resy_id = 2},
    };

    assert_resy_restaurant_list_in_db(expected);
  }

  SECTION("updates resy restaurant with same reference in db") {
    ResyRestaurant rsu{
        .restaurant_id = restaurant1.id,
        .resy_id = 1,
    };
    rsu.save();

    rsu.resy_id = 2;
    rsu.save();

    ResyRestaurantAssertion expected = {
        .restaurant_id = restaurant1.id,
        .resy_id = 2,
    };

    assert_resy_restaurant_list_in_db({expected});
  }

  SECTION("updates resy restaurant with different reference in db") {
    ResyRestaurant rsu{.restaurant_id = restaurant1.id, .resy_id = 1};
    rsu.save();

    std::optional<ResyRestaurant> other = ResyRestaurant::get(restaurant1.id);
    REQUIRE(other.has_value());

    other.value().resy_id = 2;
    other.value().save();

    ResyRestaurantAssertion expected = {
        .restaurant_id = restaurant1.id,
        .resy_id = 2,
    };

    assert_resy_restaurant_list_in_db({expected});

    rsu.refresh();

    assert_resy_restaurant(rsu, expected);
  }

  SECTION("removes resy restaurant from db") {
    ResyRestaurant rsu{
        .restaurant_id = restaurant1.id,
        .resy_id = 1,
    };
    rsu.save();

    rsu.remove();

    assert_resy_restaurant_list_in_db({});
  }

  SECTION("removes resy restaurant by restaurant_id from db") {
    ResyRestaurant rsu1{.restaurant_id = restaurant1.id, .resy_id = 1};
    rsu1.save();
    ResyRestaurant rsu2{
        .restaurant_id = restaurant2.id,
        .resy_id = 2,
    };
    rsu2.save();

    ResyRestaurant::remove(restaurant1.id);

    ResyRestaurantAssertion expected = {
        .restaurant_id = restaurant2.id,
        .resy_id = 2,
    };

    assert_resy_restaurant_list_in_db({expected});
  }

  SECTION("removes all resy restaurants from db") {
    ResyRestaurant rsu1{.restaurant_id = restaurant1.id, .resy_id = 1};
    rsu1.save();
    ResyRestaurant rsu2{.restaurant_id = restaurant2.id, .resy_id = 2};
    rsu2.save();

    ResyRestaurant::remove_all();

    assert_resy_restaurant_list_in_db({});
  }

  ResyRestaurant::drop_table();
  Restaurant::drop_table();
}
