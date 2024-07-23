#include "src/concierge/concierge.h"
#include "src/utils/cli/cli_args.h"
#include "src/utils/time/time.h"
#include <iostream>
#include <string_view>

int concierge_handler(cli_args &args) {
  if (args.size() < 3) {
    std::cout << "no concierge command specified\n";
    return 1;
  }

  std::string_view concierge_command = args[2];
  Concierge concierge{};

  if (concierge_command == "book") {
    if (args.size() < 4) {
      std::cout << "no user specified\n";
      return 1;
    }

    if (args.size() < 5) {
      std::cout << "no restaurant specified\n";
      return 1;
    }

    if (args.size() < 6) {
      std::cout << "no party size specified\n";
      return 1;
    }

    if (args.size() < 7) {
      std::cout << "no date specified\n";
      return 1;
    }

    if (args.size() < 8) {
      std::cout << "no earliest time specified\n";
      return 1;
    }

    if (args.size() < 9) {
      std::cout << "no latest time specified\n";
      return 1;
    }

    int user_id = std::stoi(std::string(args[3]));
    int restaurant_id = std::stoi(std::string(args[4]));
    int party_size = std::stoi(std::string(args[5]));
    Date date = Date(std::string(args[6]), "%y-%m-%d");
    Time earliest_time = Time(std::string(args[7]), "%H:%M");
    Time latest_time = Time(std::string(args[8]), "%H:%M");

    std::optional<User> user = User::get(user_id);
    if (!user.has_value()) {
      std::cout << "user not found\n";
      return 1;
    }

    BookingRequest request{.id = 0,
                           .user_id = user_id,
                           .restaurant_id = restaurant_id,
                           .date = date,
                           .earliest_time = earliest_time,
                           .latest_time = latest_time,
                           .party_size = party_size};

    request.save();

    bool success = concierge.attempt_booking(user.value(), request);

    if (success) {
      std::cout << "booking successful\n";
    } else {
      std::cout << "booking unsuccessful, request saved with id: " << request.id
                << "\n";
    }

    return 0;
  }

  if (concierge_command == "requests") {
    if (args.size() < 4) {
      std::cout << "no user specified\n";
      return 1;
    }

    std::vector<BookingRequest> requests =
        BookingRequest::get_by_user_id(std::stoi(std::string(args[3])));

    std::cout << "id\t\trestaurant\t\tparty size\t\tdate\t\tearliest "
                 "time\t\tlatest time\n";
    for (BookingRequest request : requests) {
      std::optional<Restaurant> restaurant =
          Restaurant::get(request.restaurant_id);
      if (!restaurant.has_value()) {
        std::cout << "restaurant not found\n";
        return 1;
      }

      std::cout << request.id << "\t\t" << restaurant.value().name << "\t\t"
                << request.party_size << "\t\t"
                << request.date.to_yy_mm_dd_string() << "\t\t"
                << request.earliest_time.to_hh_mm_ss_string() << "\t\t"
                << request.latest_time.to_hh_mm_ss_string() << "\n";
    }

    return 0;
  }

  if (concierge_command == "cancel") {
    if (args.size() < 4) {
      std::cout << "no booking request specified\n";
      return 1;
    }

    BookingRequest::remove(std::stoi(std::string(args[3])));
    return 0;
  }

  std::cout << "unknown concierge command\n";
  return 1;
}
