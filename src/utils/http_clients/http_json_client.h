#pragma once

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/json.hpp>
#include <string>
#include <string_view>

#include "src/utils/http_clients/http_client.h"

namespace asio = boost::asio;
using tcp = asio::ip::tcp;
namespace json = boost::json;

/**
  http client sending and receiving json data
*/
class HttpJsonClient : HttpClient {
 public:
  HttpJsonClient(std::string_view host, std::string_view port = "80");

  json::value get(std::string_view target);

  json::value post(std::string_view target, json::value content);
};
