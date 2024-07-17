#pragma once

#include "src/utils/http_clients/http_client.h"
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/json.hpp>
#include <string_view>
#include <unordered_map>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;
namespace json = boost::json;

/**
  http client sending and receiving json data
*/
class HttpJsonClient : HttpClient {
public:
  HttpJsonClient(std::string_view _host, std::string_view _port = "443",
                 std::unordered_map<http::field, std::string> _headers = {});

  json::value get(std::string_view target);

  json::value post_json(std::string_view target, json::value content);

  json::value post_form_data(std::string_view target, std::string_view content);
};
