#include "src/utils/http_clients/http_json_client.h"
#include "src/utils/http_clients/http_client.h"
#include <boost/json.hpp>
#include <string>
#include <string_view>
#include <unordered_map>

namespace json = boost::json;

HttpJsonClient::HttpJsonClient(
    std::string_view _host, std::string_view _port,
    std::unordered_map<http::field, std::string> _headers)
    : HttpClient(_host, _port, _headers) {}

json::value HttpJsonClient::get(std::string_view target) {
  std::string s = HttpClient::get(target);
  json::value jv = json::parse(s);
  return jv;
}

json::value HttpJsonClient::post_json(std::string_view target,
                                      json::value content) {
  std::string content_string = json::serialize(content);
  std::string s = HttpClient::post(target, content_string, "application/json");
  json::value jv = json::parse(s);
  return jv;
}

json::value HttpJsonClient::post_form_data(std::string_view target,
                                           std::string_view content) {
  std::string s =
      HttpClient::post(target, content, "application/x-www-form-urlencoded");
  json::value jv = json::parse(s);
  return jv;
}
