#pragma once

#include "src/concierge/models/booking_request.h"
#include "src/reserver/models/timeslot.h"
#include "src/reserver/models/user.h"
#include "src/reserver/reserver.h"

class Concierge {
  Reserver reserver;

private:
  void spawn_attempt_child_process(User &user, Timeslot &ts);

public:
  Concierge();

  /**
   * attempts to get a booking for a restaurant
   * we aggressively attempt to fill all timeslots
   * since user can cancel (normally), they can decide what happens if we get
   * too many
   * @returns true if booking was successful, false otherwise (request still
   * scheduled)
   */
  bool attempt_booking(User &user, BookingRequest &request);
};
