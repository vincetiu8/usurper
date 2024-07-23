#include "src/reserver/reserver.h"
#include "src/utils/cli/cli_args.h"
#include "src/utils/time/time.h"
#include <iostream>
#include <string_view>

int reserver_handler(cli_args &args) {
  if (args.size() < 3) {
    std::cout << "no reserver command specified\n";
    return 1;
  }

  std::string_view reserver_command = args[2];
  Reserver reserver{};

  if (reserver_command == "register") {
    if (args.size() < 4) {
      std::cout << "no name specified\n";
      return 1;
    }

    User user{
        .name = std::string(args[3]),
    };
    user.save();

    std::cout << "id: " << user.id << '\n';
    return 0;
  }

  if (reserver_command == "login") {
    if (args.size() < 4) {
      std::cout << "no user specified\n";
      return 1;
    }

    if (args.size() < 5) {
      std::cout << "no email specified\n";
      return 1;
    }

    if (args.size() < 6) {
      std::cout << "no password specified\n";
      return 1;
    }

    int user_id{std::stoi(std::string(args[3]))};
    std::optional<User> user = User::get(user_id);

    if (!user.has_value()) {
      std::cout << "user not found\n";
      return 1;
    }

    reserver.login(user.value(), args[4], args[5]);

    std::cout << "logged in\n";

    return 0;
  }

  if (reserver_command == "restaurants") {
    if (args.size() < 4) {
      std::cout << "no name specified\n";
      return 1;
    }

    std::vector<Restaurant> restaurants = reserver.get_restaurants(args[3]);

    std::cout << "id\tname\n";
    for (Restaurant restaurant : restaurants) {
      std::cout << restaurant.id << '\t' << restaurant.name << '\n';
    }

    return 0;
  }

  if (reserver_command == "timeslots") {
    if (args.size() < 4) {
      std::cout << "no restaurant specified\n";
      return 1;
    }

    if (args.size() < 5) {
      std::cout << "no party size specified\n";
      return 1;
    }

    if (args.size() < 6) {
      std::cout << "no date specified\n";
      return 1;
    }

    int restaurant_id{std::stoi(std::string(args[3]))};
    std::optional<Restaurant> restaurant = Restaurant::get(restaurant_id);

    if (!restaurant.has_value()) {
      std::cout << "restaurant not found\n";
      return 1;
    }

    int party_size = std::stoi(std::string(args[4]));
    Date date(std::string(args[5]), "%y-%m-%d");

    std::optional<Time> time_filter;
    if (args.size() >= 7) {
      time_filter = Time(std::string(args[6]), "%H:%M");
    }

    std::vector<Timeslot> timeslots = reserver.get_timeslots(
        restaurant.value(), party_size, date, time_filter);

    std::cout << "id\tstart_time\tend_time\n";
    for (Timeslot timeslot : timeslots) {
      std::cout << timeslot.id << '\t' << timeslot.start_time.to_hh_mm_string()
                << '\t' << timeslot.end_time.to_hh_mm_string() << '\n';
    }

    return 0;
  }

  if (reserver_command == "book") {
    if (args.size() < 4) {
      std::cout << "no user specified\n";
      return 1;
    }

    if (args.size() < 5) {
      std::cout << "no timeslot specified\n";
      return 1;
    }

    int user_id{std::stoi(std::string(args[3]))};
    std::optional<User> user = User::get(user_id);

    if (!user.has_value()) {
      std::cout << "user not found\n";
      return 1;
    }

    int timeslot_id{std::stoi(std::string(args[4]))};
    std::optional<Timeslot> timeslot = Timeslot::get(timeslot_id);
    if (!timeslot.has_value()) {
      std::cout << "timeslot not found\n";
      return 1;
    }

    reserver.book_timeslot(user.value(), timeslot.value());

    std::cout << "booked\n";
    return 0;
  }

  if (reserver_command == "bookings") {
    if (args.size() < 4) {
      std::cout << "no user specified\n";
      return 1;
    }

    int user_id = std::stoi(std::string(args[3]));
    std::vector<Booking> bookings = Booking::get_by_user_id(user_id);

    std::cout << "timeslot_id\t\trestaurant\t\tdate\t\ttime\n";
    for (Booking booking : bookings) {
      std::optional<Timeslot> timeslot = Timeslot::get(booking.timeslot_id);
      std::optional<Restaurant> restaurant =
          Restaurant::get(timeslot->restaurant_id);

      std::cout << timeslot->id << "\t\t" << restaurant->name << "\t\t"
                << timeslot->date.to_yy_mm_dd_string() << "\t\t"
                << timeslot->start_time.to_hh_mm_string() << '\n';
    }

    return 0;
  }

  if (reserver_command == "cancel") {
    if (args.size() < 4) {
      std::cout << "no user specified\n";
      return 1;
    }

    if (args.size() < 5) {
      std::cout << "no timeslot specified\n";
      return 1;
    }

    int user_id{std::stoi(std::string(args[3]))};
    std::optional<User> user = User::get(user_id);

    if (!user.has_value()) {
      std::cout << "user not found\n";
      return 1;
    }

    int timeslot_id{std::stoi(std::string(args[4]))};
    std::optional<Booking> booking = Booking::get(user_id, timeslot_id);

    if (!booking.has_value()) {
      std::cout << "booking not found\n";
      return 1;
    }

    reserver.cancel_booking(user.value(), booking.value());

    std::cout << "cancelled booking\n";
    return 0;
  }

  std::cout << "unknown reserver command " << reserver_command << '\n';
  return 1;
}
