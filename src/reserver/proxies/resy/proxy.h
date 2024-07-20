// #pragma once

// #include "src/reserver/proxies/resy/api.h"
// #include "src/reserver/reserver_models/models.h"
// #include "src/reserver/reserver_models/reservation_proxy.h"
// #include "src/reserver/reserver_models/user.h"
// #include <string_view>
// #include <vector>

// class ResyProxy : public ReservationProxy {
//   ResyApi api;

// public:
//   ResyProxy();

//   void login(User user, std::string_view email,
//              std::string_view password) override;

//   std::vector<Restaurant> query_restaurants(std::string_view name,
//                                             Locality locality) override;

//   std::vector<Timeslot> get_restaurant_timeslots(Restaurant restaurant,
//                                                  Date date) override;

//   int book_timeslot(Timeslot timeslot) override;

//   int cancel_booking(Booking booking) override;
// };
