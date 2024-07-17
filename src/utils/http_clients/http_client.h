#pragma once

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <string>
#include <string_view>
#include <unordered_map>

namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
namespace ssl = asio::ssl;
using tcp = asio::ip::tcp;

/**
 * abstract client for interacting with external http endpoints.
 * supports get and post requests.
 */
class HttpClient {
  const beast::string_view host_bs;

  asio::io_context ioc;
  ssl::stream<beast::tcp_stream> stream;
  const int http_version;

  const std::unordered_map<http::field, std::string> headers;

 protected:
  HttpClient(std::string_view _host, std::string_view _port,
             std::unordered_map<http::field, std::string> _headers);

 private:
  static ssl::stream<beast::tcp_stream> get_stream(std::string_view host,
                                                   std::string_view port);

  std::string run_request(http::request<http::string_body> req);

 protected:
  std::string get(std::string_view target);

  std::string post(std::string_view target, beast::string_view content_type);

  std::string post(std::string_view target, std::string_view content,
                   beast::string_view content_type);
};
