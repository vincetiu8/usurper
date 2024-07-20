#pragma once

#include "src/reserver/reserver_models/models.h"
#include <string>

/**
 * represents a user account with a reservation service
 */
class ReservationServiceUser {
public:
  ReservationServiceCode reservation_service_code;

  int user_id;

  // JWT token
  std::string auth_token;

  static void create_table();

  static void drop_table();

  static std::optional<ReservationServiceUser>
  get(ReservationServiceCode reservation_service_code, int user_id);

  static std::vector<ReservationServiceUser> get_all();

  void create();

  void update();

  void save();

  void refresh();

  void remove();

  static void remove(ReservationServiceCode reservation_service_code,
                     int user_id);

  static void remove_by_user_id(int user_id);

  static void remove_all();
};
