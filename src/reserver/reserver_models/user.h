#pragma once

#include <optional>
#include <string>

/**
 * wrapper around `User` adding methods to interact with database
 */
class User {
public:
  // primary key
  int id;

  std::string name;

  static void create_table();

  static void drop_table();

  static std::optional<User> get(int id);

  static std::vector<User> get_all();

  void create();

  void update();

  void save();

  void refresh();

  void remove();

  static void remove(int id);

  static void remove_all();
};