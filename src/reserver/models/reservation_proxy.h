#pragma once

#include "src/reserver/models/booking.h"
#include "src/reserver/models/restaurant.h"
#include "src/reserver/models/timeslot.h"
#include "src/reserver/models/user.h"
#include <optional>
#include <string_view>
#include <vector>

class ReservationProxy {
public:
  virtual void login(User &user, std::string_view email,
                     std::string_view password) = 0;

  virtual std::vector<Restaurant> get_restaurants(std::string_view name) = 0;

  virtual std::vector<Timeslot>
  get_timeslots(Restaurant &restaurant, int party_size, Date &date,
                std::optional<Time> time_filter) = 0;

  virtual std::optional<Booking> book_timeslot(User &user,
                                               Timeslot &timeslot) = 0;

  virtual void cancel_booking(User &user, Booking &booking) = 0;
};
