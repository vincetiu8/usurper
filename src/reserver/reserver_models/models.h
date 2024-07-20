#pragma once

#include "src/reserver/reserver_models/reservation_service_code.h"
#include "src/utils/time/time.h"
#include <string>

/**
 * represents a unique restaurant
 */
class Restaurant {
public:
  // primary key
  int id;

  ReservationServiceCode reservation_service_code;
};

/**
 * represents a unique timeslot at a restaurant
 */
class Timeslot {
public:
  // primary key
  int id;

  int restaurant_id;

  Date date;

  Time time;

  int party_size;

  bool available;
};

/**
 * represents a user timeslot booking
 */
class Booking {
public:
  int timeslot_id;

  int user_id;
};
