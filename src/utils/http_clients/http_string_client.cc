#include "src/utils/http_clients/http_string_client.h"

#include <string>
#include <string_view>

#include "src/utils/http_clients/http_client.h"

HttpStringClient::HttpStringClient(std::string_view host, std::string_view port)
    : HttpClient(host, port) {}

std::string HttpStringClient::get(std::string_view target) {
  return HttpClient::get(target);
}

std::string HttpStringClient::post(std::string_view target,
                                   std::string content) {
  return HttpClient::post(target, content, "text/html");
}
