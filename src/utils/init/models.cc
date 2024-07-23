#include "src/concierge/models/booking_request.h"
#include "src/reserver/models/booking.h"
#include "src/reserver/models/restaurant.h"
#include "src/reserver/models/timeslot.h"
#include "src/reserver/models/user.h"

void init_models() {
  User::create_table();
  Restaurant::create_table();
  Timeslot::create_table();
  Booking::create_table();
  BookingRequest::create_table();
}

void reset_models() {
  User::drop_table();
  Restaurant::drop_table();
  Timeslot::drop_table();
  Booking::drop_table();
  BookingRequest::drop_table();

  init_models();
}
