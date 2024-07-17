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
    std::cout << "output token: " << output.token << '\n';
    return 0;
  }

  if (resy_command == "search") {
  }

  if (resy_command == "find") {
  }

  if (resy_command == "details") {
  }

  if (resy_command == "book") {
  }

  std::cout << "unknown resy command " << resy_command << '\n';
  return 1;
}
