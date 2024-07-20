#include "src/reserver/reserver_models/booking.h"
#include "src/reserver/reserver_models/restaurant.h"
#include "src/reserver/reserver_models/timeslot.h"
#include "src/reserver/reserver_models/user.h"

class Reserver {
public:
  virtual void login(User user, std::string_view email,
                     std::string_view password);

  virtual std::vector<Restaurant> query_restaurants(std::string_view name);

  virtual std::vector<Timeslot> get_restaurant_timeslots(Restaurant restaurant,
                                                         Date date);

  virtual int book_timeslot(Timeslot timeslot);

  virtual int cancel_booking(Booking booking);
};
