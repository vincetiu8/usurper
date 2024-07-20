#pragma once

#include <optional>
#include <string>
#include <vector>

/**
 * represents a unique restaurant
 */
class Restaurant {
public:
  // primary key
  int id;

  std::string name;

  static void create_table();

  static void drop_table();

  static std::optional<Restaurant> get(int id);

  static std::vector<Restaurant> get_by_name(std::string &name);

  static std::vector<Restaurant> get_all();

  void create();

  void update();

  void save();

  void refresh();

  void remove();

  static void remove(int id);

  static void remove_all();
};