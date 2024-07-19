#pragma once

#include "src/utils/http_clients/http_client.h"
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <string>
#include <string_view>
#include <unordered_map>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

/**
  http client sending and receiving string data
*/
class HttpStringClient : public HttpClient {
public:
  HttpStringClient(std::string_view _host, std::string_view _port = "443",
                   std::unordered_map<std::string, std::string> _headers = {});

  std::string get(std::string_view target, headers_t headers);

  std::string post(std::string_view target, std::string_view content,
                   headers_t headers);
};
