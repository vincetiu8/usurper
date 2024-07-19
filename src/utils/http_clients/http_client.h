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

using headers_t = std::unordered_map<std::string, std::string>;

/**
 * abstract client for interacting with external http endpoints.
 * supports get and post requests.
 */
class HttpClient {
  const beast::string_view host_bs;

  asio::io_context ioc;
  ssl::context ctx;
  ssl::stream<beast::tcp_stream> stream;
  const int http_version;

  headers_t headers;

protected:
  HttpClient(std::string_view _host, std::string_view _port,
             headers_t _headers);

  ~HttpClient();

private:
  static ssl::stream<beast::tcp_stream> get_stream(std::string_view host,
                                                   std::string_view port,
                                                   asio::io_context &ioc,
                                                   ssl::context &ctx);

  std::string run_request(http::request<http::string_body> &req,
                          headers_t _headers);

protected:
  std::string get(std::string_view target, headers_t headers);

  std::string post(std::string_view target, headers_t headers);

  std::string post(std::string_view target, std::string_view content,
                   headers_t headers);
};
