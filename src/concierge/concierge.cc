#include "src/concierge/concierge.h"
#include <iostream>
#include <unistd.h>

Concierge::Concierge() : reserver() {}

bool Concierge::attempt_booking(User &user, BookingRequest &request) {
  std::optional<Restaurant> restaurant = Restaurant::get(request.restaurant_id);
  if (!restaurant.has_value()) {
    throw std::runtime_error("restaurant not found");
  }

  std::vector<Timeslot> timeslots =
      reserver.get_timeslots(restaurant.value(), request.party_size,
                             request.date, request.earliest_time);

  for (Timeslot timeslot : timeslots) {
    if (timeslot.start_time < request.earliest_time ||
        timeslot.start_time > request.latest_time) {
      // not in range
      continue;
    }

    std::optional<Booking> booking = reserver.book_timeslot(user, timeslot);

    if (booking.has_value()) {
      request.remove();
      return true;
    }
  }

  return false;
}