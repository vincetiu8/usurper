#pragma once

#include "src/reserver/proxies/resy/api.h"
#include "src/reserver/reserver_models/booking.h"
#include "src/reserver/reserver_models/restaurant.h"
#include "src/reserver/reserver_models/timeslot.h"
#include "src/reserver/reserver_models/user.h"
#include <string_view>
#include <vector>

class ResyProxy {
  ResyApi api;

public:
  ResyProxy();

  void login(User user, std::string_view email, std::string_view password);

  std::vector<Restaurant> query_restaurants(std::string_view name);

  std::vector<Timeslot> get_restaurant_timeslots(Restaurant restaurant,
                                                 Date date);

  int book_timeslot(Timeslot timeslot);

  int cancel_booking(Booking booking);
};
