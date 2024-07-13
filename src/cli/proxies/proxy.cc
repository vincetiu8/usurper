#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "src/cli/proxies/resy.h"
#include "src/utils/cli/cli_args.h"
#include "src/utils/proxy/http_client.h"

int http_handler(cli_args& args) {
  if (args.size() < 4) {
    std::cout << "no method specified" << '\n';
    return 1;
  }

  if (args.size() < 5) {
    std::cout << "no host specified" << '\n';
    return 1;
  }

  std::string_view method = args[3];
  std::string_view host = args[4];

  std::string_view target = "";
  if (args.size() >= 6) {
    target = args[5];
  }

  HttpClient http_client{host};

  if (method == "get") {
    std::string s = http_client.get(target);
    std::cout << s << '\n';
    return 0;
  }

  if (method == "post") {
    std::string s = http_client.post(target);
    std::cout << s << '\n';
    return 0;
  }

  std::cout << "unknown http method " << method << '\n';
  return 1;
}

int proxy_handler(cli_args& args) {
  if (args.size() < 3) {
    std::cout << "no proxy specified" << '\n';
    return 1;
  }

  std::string_view proxy_name = args[2];

  if (proxy_name == "http") {
    return http_handler(args);
  }

  if (proxy_name == "resy") {
    return resy_handler(args);
  }

  std::cout << "unknown proxy " << proxy_name << '\n';
  return 1;
}
