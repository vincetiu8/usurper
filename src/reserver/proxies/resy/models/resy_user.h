#pragma once

#include <string>
#include <vector>

/**
 * represents a user account with a reservation service
 */
class ResyUser {
public:
  int user_id;

  // JWT token
  std::string auth_token;

  static void create_table();

  static void drop_table();

  static std::optional<ResyUser> get(int user_id);

  static std::vector<ResyUser> get_all();

  void create();

  void update();

  void save();

  void refresh();

  void remove();

  static void remove(int user_id);

  static void remove_all();
};
