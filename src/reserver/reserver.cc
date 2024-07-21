#include "src/reserver/reserver.h"
#include "src/reserver/models/booking.h"
#include "src/reserver/models/reservation_proxy.h"
#include "src/reserver/models/restaurant.h"
#include "src/reserver/models/timeslot.h"
#include "src/reserver/models/user.h"
#include "src/reserver/proxies/resy/proxy.h"
#include <string_view>
#include <vector>

Reserver::Reserver() : resy_proxy(ResyProxy()) {}

void Reserver::login(User user, std::string_view email,
                     std::string_view password) {
  resy_proxy.login(user, email, password);
}

std::vector<Restaurant> Reserver::query_restaurants(std::string_view name) {
  return resy_proxy.query_restaurants(name);
}

std::vector<Timeslot> Reserver::get_restaurant_timeslots(Restaurant restaurant,
                                                         int party_size,
                                                         Date date) {
  return resy_proxy.get_restaurant_timeslots(restaurant, party_size, date);
}

void Reserver::book_timeslot(User user, Timeslot timeslot) {
  resy_proxy.book_timeslot(user, timeslot);
}

void Reserver::cancel_booking(User user, Booking booking) {
  resy_proxy.cancel_booking(user, booking);
}
