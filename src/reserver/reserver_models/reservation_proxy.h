#pragma once

#include <vector>

#include "models.h"

class ReservationProxy {
 public:
  std::vector<Restaurant> filter_restaurants(std::string_view name,
                                             Locality locality);

  std::vector<Timeslot> get_timeslots(Restaurant restaurant);

  int book_timeslot(Timeslot timeslot);

  int cancel_booking(Booking booking);

  int login(User user, std::string_view email, std::string_view password);
};
