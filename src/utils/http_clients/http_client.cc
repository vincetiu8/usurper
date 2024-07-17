#include "src/utils/http_clients/http_client.h"

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>

#include "src/utils/http_clients/root_certificates.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
namespace ssl = asio::ssl;
using tcp = asio::ip::tcp;

HttpClient::HttpClient(std::string_view _host, std::string_view _port,
                       std::unordered_map<http::field, std::string> _headers)
    : host_bs{_host.data(), _host.size()},
      stream{get_stream(_host, _port)},
      http_version{11},
      headers{_headers} {}

ssl::stream<beast::tcp_stream> HttpClient::get_stream(std::string_view host,
                                                      std::string_view port) {
  // we assume the servers all use SSL
  asio::io_context ioc;

  ssl::context ctx{ssl::context::tls};

  ctx.set_default_verify_paths();

  load_root_certificates(ctx);

  ctx.set_verify_mode(ssl::verify_peer);

  tcp::resolver resolver(ioc);
  ssl::stream<beast::tcp_stream> stream(ioc, ctx);

  // Set SNI Hostname (many hosts need this to handshake successfully)
  if (!SSL_set_tlsext_host_name(stream.native_handle(),
                                std::string(host).c_str())) {
    throw boost::system::system_error(::ERR_get_error(),
                                      boost::asio::error::get_ssl_category());
  }

  auto const results = resolver.resolve(host, port);

  beast::get_lowest_layer(stream).connect(results);

  stream.handshake(ssl::stream_base::client);

  return stream;
}

std::string HttpClient::run_request(http::request<http::string_body> req) {
  for (auto& [field, value] : headers) {
    req.set(field, value);
  }

  http::write(stream, req);

  beast::flat_buffer buffer;
  http::response<http::dynamic_body> res;

  http::read(stream, buffer, res);

  std::string s{};
  for (auto seq : res.body().data()) {
    auto* cbuf = boost::asio::buffer_cast<const char*>(seq);
    s.append(cbuf, boost::asio::buffer_size(seq));
  }

  return s;
}

std::string HttpClient::get(std::string_view target) {
  if (target.length() == 0) {
    target = "/";
  }

  beast::string_view target_bs{target.data(), target.size()};
  http::request<http::string_body> req{http::verb::get, target_bs,
                                       http_version};
  req.set(http::field::host, host_bs);

  return run_request(req);
}

std::string HttpClient::post(std::string_view target,
                             beast::string_view content_type) {
  if (target.length() == 0) {
    target = "/";
  }

  beast::string_view target_bs{target.data(), target.size()};

  http::request<http::string_body> req{http::verb::post, target_bs,
                                       http_version};

  req.set(http::field::host, host_bs);
  req.set(http::field::content_type, content_type);

  return run_request(req);
}

std::string HttpClient::post(std::string_view target, std::string_view content,
                             beast::string_view content_type) {
  if (target.length() == 0) {
    target = "/";
  }

  beast::string_view target_bs{target.data(), target.size()};

  http::request<http::string_body> req{http::verb::post, target_bs,
                                       http_version, content};

  req.set(http::field::host, host_bs);
  req.set(http::field::content_type, content_type);
  req.prepare_payload();

  return run_request(req);
}
