#include "src/reserver/proxies/resy/api.h"
#include "src/utils/cli/cli_args.h"
#include <iostream>
#include <string_view>

int resy_handler(cli_args &args) {
  if (args.size() < 4) {
    std::cout << "no resy command specified" << '\n';
    return 1;
  }

  std::string_view resy_command = args[3];
  ResyApi api{};

  if (resy_command == "login") {
    if (args.size() < 5) {
      std::cout << "no email specified" << '\n';
      return 1;
    }

    if (args.size() < 6) {
      std::cout << "no password specified" << '\n';
      return 1;
    }

    ResyApi::LoginInput input{.email = args[4], .password = args[5]};

    ResyApi::LoginOutput output = api.login(input);
    std::cout << "output token: " << output.auth_token << '\n';
    return 0;
  }

  if (resy_command == "search") {
    if (args.size() < 5) {
      std::cout << "no latitude specified" << '\n';
      return 1;
    }

    if (args.size() < 6) {
      std::cout << "no longitude specified" << '\n';
      return 1;
    }

    if (args.size() < 7) {
      std::cout << "no query specified" << '\n';
      return 1;
    }

    ResyApi::SearchInput input{
        .geo = {.latitude = std::stod(std::string(args[4])),
                .longitude = std::stod(std::string(args[5]))},
        .query = args[6],
    };

    ResyApi::SearchOutput output = api.search(input);
    for (const ResyApi::SearchOutputHit &hit : output.hits) {
      std::cout << "hit name: " << hit.name << "\tid: " << hit.id << '\n';
    }
    return 0;
  }

  if (resy_command == "find") {
    if (args.size() < 5) {
      std::cout << "no venue id specified" << '\n';
      return 1;
    }

    if (args.size() < 6) {
      std::cout << "no party size specified" << '\n';
      return 1;
    }

    if (args.size() < 7) {
      std::cout << "no date specified" << '\n';
      return 1;
    }

    std::optional<Time> time_filter =
        args.size() < 8
            ? std::nullopt
            : std::optional<Time>(Time(std::string(args[7]), "%H:%M"));

    ResyApi::FindInput input{
        .venue_id = std::stoi(std::string(args[4])),
        .party_size = std::stoi(std::string(args[5])),
        .date = Date(std::string(args[6]), "%y-%m-%d"),
        .time_filter = time_filter,
    };

    ResyApi::FindOutput output = api.find(input);

    for (ResyApi::FindOutputSlot &slot : output.slots) {
      std::cout << "slot time: " << slot.start_time.to_hh_mm_string()
                << "\tid: " << slot.id << "\ttimeslot_token: " << slot.token
                << '\n';
    }
    return 0;
  }

  if (resy_command == "details") {
    if (args.size() < 5) {
      std::cout << "no auth token specified" << '\n';
      return 1;
    }

    if (args.size() < 6) {
      std::cout << "no timeslot token specified" << '\n';
      return 1;
    }

    if (args.size() < 7) {
      std::cout << "no date specified" << '\n';
      return 1;
    }

    if (args.size() < 8) {
      std::cout << "no party size specified" << '\n';
      return 1;
    }

    ResyApi::DetailsInput input{
        .auth_token = args[4],
        .timeslot_token = args[5],
        .day = Date(std::string(args[6]), "%y-%m-%d"),
        .party_size = std::stoi(std::string(args[7])),
    };

    std::optional<ResyApi::DetailsOutput> output = api.details(input);

    if (!output.has_value()) {
      std::cout << "details failed" << '\n';
      return 1;
    }

    std::cout << "book token: " << output->book_token << '\n';
    return 0;
  }

  if (resy_command == "book") {
    if (args.size() < 5) {
      std::cout << "no auth token specified" << '\n';
      return 1;
    }

    if (args.size() < 6) {
      std::cout << "no book token specified" << '\n';
      return 1;
    }

    ResyApi::BookInput input{
        .auth_token = args[4],
        .book_token = args[5],
    };

    std::optional<ResyApi::BookOutput> output = api.book(input);

    if (!output.has_value()) {
      std::cout << "booking failed" << '\n';
      return 1;
    }

    std::cout << "booking token: " << output->booking_token
              << "\treservation id: " << output->reservation_id << '\n';
    return 0;
  }

  if (resy_command == "cancel") {
    if (args.size() < 5) {
      std::cout << "no auth token specified" << '\n';
      return 1;
    }

    if (args.size() < 6) {
      std::cout << "no booking token specified" << '\n';
      return 1;
    }

    ResyApi::CancelInput input{
        .auth_token = args[4],
        .booking_token = args[5],
    };

    api.cancel(input);
    std::cout << "cancelled" << '\n';
    return 0;
  }

  std::cout << "unknown resy command " << resy_command << '\n';
  return 1;
}
