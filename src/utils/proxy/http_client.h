#pragma once

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <string>
#include <string_view>

namespace beast = boost::beast;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

/**
  client for interacting with external http endpoints.
  supports get and post requests.
*/
class HttpClient {
  const beast::string_view host_bs;

  asio::io_context ioc;
  tcp::resolver resolver;
  beast::tcp_stream stream;
  const int http_version;

 public:
  HttpClient(std::string_view host, std::string_view port = "80");

  std::string get(std::string_view target);

  std::string post(std::string_view target);
};
