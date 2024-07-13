#include "src/utils/http_clients/http_json_client.h"

#include <boost/json.hpp>
#include <string>
#include <string_view>

#include "src/utils/http_clients/http_client.h"

namespace json = boost::json;

HttpJsonClient::HttpJsonClient(std::string_view host, std::string_view port)
    : HttpClient(host, port) {}

json::value HttpJsonClient::get(std::string_view target) {
  std::string s = HttpClient::get(target);
  json::value jv = json::parse(s);
  return jv;
}

json::value HttpJsonClient::post(std::string_view target, json::value content) {
  std::string content_string = json::serialize(content);
  std::string s = HttpClient::post(target, content_string, "application/json");
  json::value jv = json::parse(s);
  return jv;
}
