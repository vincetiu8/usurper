#pragma once

#include "src/reserver/models/booking.h"
#include "src/reserver/models/restaurant.h"
#include "src/reserver/models/timeslot.h"
#include "src/reserver/models/user.h"
#include <string_view>
#include <vector>

class ReservationProxy {
public:
  virtual void login(User user, std::string_view email,
                     std::string_view password);

  virtual std::vector<Restaurant> query_restaurants(std::string_view name);

  virtual std::vector<Timeslot>
  get_restaurant_timeslots(Restaurant restaurant, int party_size, Date date);

  virtual void book_timeslot(User user, Timeslot timeslot);

  virtual void cancel_booking(User user, Booking booking);
};
