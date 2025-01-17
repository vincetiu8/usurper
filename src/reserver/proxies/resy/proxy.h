#pragma once

#include "src/reserver/models/booking.h"
#include "src/reserver/models/reservation_proxy.h"
#include "src/reserver/models/restaurant.h"
#include "src/reserver/models/timeslot.h"
#include "src/reserver/models/user.h"
#include "src/reserver/proxies/resy/api.h"
#include <optional>
#include <string_view>
#include <vector>

class ResyProxy : public ReservationProxy {
  ResyApi api;

public:
  ResyProxy();

  void login(User &user, std::string_view email,
             std::string_view password) override;

  std::vector<Restaurant> get_restaurants(std::string_view name) override;

  std::vector<Timeslot> get_timeslots(Restaurant &restaurant, int party_size,
                                      Date &date,
                                      std::optional<Time> time_filter) override;

  std::optional<Booking> book_timeslot(User &user, Timeslot &timeslot) override;

  void cancel_booking(User &user, Booking &booking) override;
};
