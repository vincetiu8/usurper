#include "src/booker/proxies/resy/resy.h"

#include <iostream>
#include <string_view>

#include "src/utils/cli/cli_args.h"

int resy_handler(cli_args& args) {
  if (args.size() < 4) {
    std::cout << "no resy command specified" << '\n';
    return 1;
  }

  std::string_view resy_command = args[3];

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
