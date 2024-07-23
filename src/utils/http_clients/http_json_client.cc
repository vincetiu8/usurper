#include "src/utils/http_clients/http_json_client.h"
#include "src/utils/http_clients/http_client.h"
#include <boost/json.hpp>
#include <string>
#include <string_view>
#include <unordered_map>

namespace json = boost::json;

HttpJsonClient::HttpJsonClient(
    std::string_view _host, std::string_view _port,
    std::unordered_map<std::string, std::string> _headers)
    : HttpClient(_host, _port, _headers) {}

json::value HttpJsonClient::get(std::string_view target, headers_t headers) {
  std::string s = HttpClient::get(target, headers);

  if (s.size() == 0) {
    return json::string("");
  }

  json::value jv = json::parse(s);
  return jv;
}

json::value HttpJsonClient::post_json(std::string_view target,
                                      json::value content, headers_t headers) {
  std::string content_string = json::serialize(content);
  headers["Content-Type"] = "application/json";
  std::string s = HttpClient::post(target, content_string, headers);

  if (s.size() == 0) {
    return json::string("");
  }

  json::value jv = json::parse(s);
  return jv;
}

json::value HttpJsonClient::post_form_data(std::string_view target,
                                           std::string_view content,
                                           headers_t headers) {
  headers["Content-Type"] = "application/x-www-form-urlencoded";
  std::string s = HttpClient::post(target, content, headers);

  if (s.size() == 0) {
    return json::string("");
  }

  json::value jv = json::parse(s);
  return jv;
}
