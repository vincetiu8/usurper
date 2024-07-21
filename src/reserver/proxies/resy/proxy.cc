#include "src/reserver/proxies/resy/proxy.h"
#include "src/reserver/models/restaurant.h"
#include "src/reserver/models/timeslot.h"
#include "src/reserver/proxies/resy/api.h"
#include <string_view>

ResyProxy::ResyProxy() : api{ResyApi()} {};

void ResyProxy::login(User user, std::string_view email,
                      std::string_view password) {
  ResyApi::LoginInput login_input{.email = email, .password = password};

  ResyApi::LoginOutput login_output = api.login(login_input);

  user.resy_token = login_output.auth_token;
  user.save();
}

std::vector<Restaurant> ResyProxy::query_restaurants(std::string_view name) {
  ResyApi::SearchInput search_input{
      .geo = new_york,
      .query = name,
  };

  ResyApi::SearchOutput search_output = api.search(search_input);

  std::vector<Restaurant> restaurants;

  for (const ResyApi::SearchOutputHit &hit : search_output.hits) {
    std::optional<Restaurant> result = Restaurant::get_by_resy_id(hit.id);

    if (result.has_value()) {
      restaurants.push_back(result.value());
      continue;
    }

    Restaurant restaurant{
        .name = hit.name,
        .rsc = ReservationServiceCode::resy,
        .resy_id = hit.id,
    };

    restaurant.save();

    restaurants.push_back(restaurant);
  }

  return restaurants;
}

std::vector<Timeslot> ResyProxy::get_restaurant_timeslots(Restaurant restaurant,
                                                          int party_size,
                                                          Date date) {
  ResyApi::FindInput find_input{
      .venue_id = restaurant.resy_id.value(),
      .party_size = 2,
      .date = date,
  };

  ResyApi::FindOutput find_output = api.find(find_input);

  std::vector<Timeslot> timeslots;

  for (const ResyApi::FindOutputSlot &slot : find_output.slots) {
    std::optional<Timeslot> result =
        Timeslot::get_by_restaurant_id_date_start_time(restaurant.id, date,
                                                       slot.start_time);

    if (result.has_value()) {
      timeslots.push_back(result.value());
      continue;
    }

    Timeslot timeslot{
        .restaurant_id = restaurant.id,
        .date = date,
        .start_time = slot.start_time,
        .end_time = slot.end_time,
        .party_size = party_size,
        .available = true,
        .resy_token = slot.token,
    };
    timeslot.save();

    timeslots.push_back(timeslot);
  }

  return timeslots;
}

void ResyProxy::book_timeslot(User user, Timeslot timeslot) {
  ResyApi::DetailsInput details_input{
      .auth_token = user.resy_token.value(),
      .timeslot_token = timeslot.resy_token.value(),
      .day = timeslot.date,
      .party_size = timeslot.party_size,
  };

  ResyApi::DetailsOutput details_output = api.details(details_input);

  ResyApi::BookInput book_input{
      .auth_token = user.resy_token.value(),
      .book_token = details_output.book_token,
  };

  ResyApi::BookOutput book_output = api.book(book_input);

  Booking booking{
      .user_id = user.id,
      .timeslot_id = timeslot.id,
      .resy_token = book_output.booking_token,
  };

  booking.save();
}

void ResyProxy::cancel_booking(User user, Booking booking) {
  ResyApi::CancelInput cancel_input{
      .auth_token = user.resy_token.value(),
      .booking_token = booking.resy_token.value(),
  };

  api.cancel(cancel_input);
}
