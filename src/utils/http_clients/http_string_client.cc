#include "src/utils/http_clients/http_string_client.h"
#include "src/utils/http_clients/http_client.h"
#include <string>
#include <string_view>
#include <unordered_map>

HttpStringClient::HttpStringClient(
    std::string_view _host, std::string_view _port,
    std::unordered_map<http::field, std::string> _headers)
    : HttpClient(_host, _port, _headers) {}

std::string HttpStringClient::get(std::string_view target) {
  return HttpClient::get(target);
}

std::string HttpStringClient::post(std::string_view target,
                                   std::string_view content) {
  return HttpClient::post(target, content, "text/html");
}
