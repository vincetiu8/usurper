#include <iostream>
#include <string>
#include <string_view>

#include "src/cli/proxies/http_clients.h"
#include "src/cli/proxies/resy.h"
#include "src/utils/cli/cli_args.h"

int proxy_handler(cli_args& args) {
  if (args.size() < 3) {
    std::cout << "no proxy specified" << '\n';
    return 1;
  }

  std::string_view proxy_name = args[2];

  if (proxy_name == "http" || proxy_name == "http-json") {
    return http_json_handler(args);
  }

  if (proxy_name == "http-string") {
    return http_string_handler(args);
  }

  if (proxy_name == "resy") {
    return resy_handler(args);
  }

  std::cout << "unknown proxy " << proxy_name << '\n';
  return 1;
}
