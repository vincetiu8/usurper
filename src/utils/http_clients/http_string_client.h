#pragma once

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <string>
#include <string_view>

#include "src/utils/http_clients/http_client.h"

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

/**
  http client sending and receiving string data
*/
class HttpStringClient : HttpClient {
 public:
  HttpStringClient(std::string_view host, std::string_view port = "80");

  std::string get(std::string_view target);

  std::string post(std::string_view target, std::string content);
};
