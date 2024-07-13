#pragma once

#include <string>

#include "src/utils/geo/geo.h"
#include "src/utils/time/time.h"

/**
 * represents a unique user in the system
 */
class User {
 public:
  // primary key
  int id;

  std::string name;
};

class ReservationService {
 public:
  // primary key
  std::string code;

  std::string name;
};

class ReservationServiceUser {
 public:
  ReservationService reservation_service;

  User user;

  // JWT token
  std::string token;
};

class Cuisine {
 public:
  // primary key
  std::string name;
};

class Restaurant {
 public:
  // primary key
  // int id;
  std::string name;

  std::string locality_name;

  Locality locality;

  std::string cuisine_name;

  Cuisine cuisine;

  std::string reservation_service_code;

  // currently we only support one reservation service per restaurant
  ReservationService reservation_service;
};

class Timeslot {
 public:
  // primary key
  int id;

  int restaurant_id;

  Restaurant restaurant;

  Date date;

  Time time;

  int party_size;

  bool available;
};

class Booking {
 public:
  int timeslot_id;
  Timeslot timeslot;

  int user_id;
  User user;
};
