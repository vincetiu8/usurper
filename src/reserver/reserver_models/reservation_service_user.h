#pragma once

#include "src/reserver/reserver_models/reservation_service_code.h"
#include <string>

/**
 * represents a user account with a reservation service
 */
class ReservationServiceUser {
public:
  int user_id;

  ReservationServiceCode reservation_service_code;

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
