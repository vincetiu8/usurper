#pragma once

#include <string>
#include <vector>

/**
 * represents a user account with a reservation service
 */
class ResyRestaurant {
public:
  int restaurant_id;

  int resy_id;

  static void create_table();

  static void drop_table();

  static std::optional<ResyRestaurant> get(int restaurant_id);

  static std::vector<ResyRestaurant> get_all();

  void create();

  void update();

  void save();

  void refresh();

  void remove();

  static void remove(int restaurant_id);

  static void remove_all();
};
