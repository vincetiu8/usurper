#include "src/reserver/proxies/resy/proxy.h"
#include "src/reserver/models/restaurant.h"
#include "src/reserver/models/timeslot.h"
#include "src/reserver/proxies/resy/api.h"
#include <string_view>

ResyProxy::ResyProxy() : api{ResyApi()} {};

// void ResyProxy::login(User user, std::string_view email,
//                       std::string_view password) {
//   ResyApi::LoginInput input{.email = email, .password = password};

//   ResyApi::LoginOutput res = api.login(input);

//   ResyUser resy_user{.user_id = user.id, .auth_token = res.auth_token};
//   resy_user.save();
// }

// std::vector<Restaurant> ResyProxy::query_restaurants(std::string_view name) {
//   ResyApi::SearchInput input{
//       .geo = new_york,
//       .query = name,
//   };

//   ResyApi::SearchOutput res = api.search(input);

//   std::vector<Restaurant> restaurants;

//   for (const ResyApi::SearchOutputHit &hit : res.hits) {
//     std::optional<ResyRestaurant> result =
//         ResyRestaurant::get_by_resy_id(hit.id);

//     if (!result.has_value()) {
//       std::optional<Restaurant> restaurant =
//           Restaurant::get(result->restaurant_id);

//       if (!restaurant.has_value()) {
//         throw std::runtime_error(
//             "resy restaurant present but restaurant not found");
//       }

//       restaurants.push_back(restaurant.value());
//       continue;
//     }

//     Restaurant restaurant{
//         .name = hit.name,
//         .rsc = ReservationServiceCode::resy,
//     };

//     restaurant.save();

//     ResyRestaurant resy_restaurant{
//         .restaurant_id = restaurant.id,
//         .resy_id = hit.id,
//     };

//     resy_restaurant.save();

//     restaurants.push_back(restaurant);
//   }

//   return restaurants;
// }

// std::vector<Timeslot> ResyProxy::get_restaurant_timeslots(Restaurant
// restaurant,
//                                                           int party_size,
//                                                           Date date) {
//   std::optional<ResyRestaurant> resy_restaurant =
//       ResyRestaurant::get(restaurant.id);

//   if (!resy_restaurant.has_value()) {
//     throw std::runtime_error("resy restaurant not found");
//   }

//   ResyApi::FindInput input{
//       .venue_id = resy_restaurant->resy_id,
//       .party_size = 2,
//       .date = date,
//   };

//   ResyApi::FindOutput res = api.find(input);

//   std::vector<Timeslot> timeslots;

//   for (const ResyApi::FindOutputSlot &slot : res.slots) {
//     std::optional<Timeslot> result =
//         Timeslot::get_by_restaurant_id_date_start_time(restaurant.id, date,
//                                                        slot.start_time);

//     if (result.has_value()) {
//       timeslots.push_back(result.value());
//       continue;
//     }

//     Timeslot timeslot{
//         .restaurant_id = restaurant.id,
//         .date = date,
//         .start_time = slot.start_time,
//         .end_time = slot.end_time,
//         .party_size = party_size,
//         .available = true,
//     };
//     timeslot.save();

//     timeslots.push_back(timeslot);
//   }

//   return timeslots;
// }

// int ResyProxy::book_timeslot(Timeslot timeslot) {}