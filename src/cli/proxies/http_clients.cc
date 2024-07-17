#include "src/utils/cli/cli_args.h"
#include "src/utils/http_clients/http_json_client.h"
#include "src/utils/http_clients/http_string_client.h"
#include <iostream>

int http_string_handler(cli_args &args) {
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

  HttpStringClient http_client{host};

  if (method == "get") {
    std::string s = http_client.get(target);
    std::cout << s << '\n';
    return 0;
  }

  if (method == "post") {
    std::string_view content = "";
    if (args.size() >= 7) {
      content = args[6];
    }

    std::string s = http_client.post(target, content);
    std::cout << s << '\n';
    return 0;
  }

  std::cout << "unknown http method " << method << '\n';
  return 1;
}

int http_json_handler(cli_args &args) {
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

  HttpJsonClient http_client{host};

  if (method == "get") {
    json::value s = http_client.get(target);
    std::cout << s << '\n';
    return 0;
  }

  if (method == "post") {
    std::string_view content = "";
    if (args.size() >= 7) {
      content = args[6];
    }

    json::value content_json = json::parse(content);

    json::value s = http_client.post_json(target, content_json);
    std::cout << s << '\n';
    return 0;
  }

  if (method == "post-form-data") {
    if (args.size() < 7) {
      std::cout << "no content specified" << '\n';
      return 1;
    }

    std::string_view content = args[6];

    json::value s = http_client.post_form_data(target, content);
    std::cout << s << '\n';
    return 0;
  }

  std::cout << "unknown http method " << method << '\n';
  return 1;
}
