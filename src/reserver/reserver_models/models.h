#pragma once

#include <chrono>
#include <string>

#include "src/utils/geo/geo.h"

// namespace chrono = std::chrono;

/**
 * represents a unique user in the system
 */
class User {
  // int id;  // primary key
  std::string name;
};

class ReservationService {
  std::string code;  // primrary key
  std::string name;
};

class ReservationServiceUser {
  ReservationService reservation_service;
  User user;
  std::string token;  // JWT token
};

class Cuisine {
  std::string name;  // primary key
};

class Restaurant {
  // int id;  // primary key
  std::string name;
  std::string locality_name;
  Locality locality;
  std::string cuisine_name;
  Cuisine cuisine;
  std::string reservation_service_code;
  ReservationService reservation_service;  // currently we only support one
                                           // reservation service per restaurant
};

class Timeslot {
  // int id;
  // int restaurant_id;
  Restaurant restaurant;
  // chrono::year_month_day date;
};
