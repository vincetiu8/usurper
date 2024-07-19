#include "src/utils/http_clients/http_client.h"
#include "src/utils/http_clients/root_certificates.h"
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <iostream>
#include <string>
#include <string_view>

namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
namespace ssl = asio::ssl;
using tcp = asio::ip::tcp;

HttpClient::HttpClient(std::string_view _host, std::string_view _port,
                       headers_t _headers)
    : host_bs{_host.data(), _host.size()}, ioc{},
      ctx{ssl::context::tlsv13_client},
      stream{get_stream(_host, _port, ioc, ctx)}, http_version{11},
      headers{_headers} {}

ssl::stream<beast::tcp_stream> HttpClient::get_stream(std::string_view host,
                                                      std::string_view port,
                                                      asio::io_context &ioc,
                                                      ssl::context &ctx) {
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

HttpClient::~HttpClient() {
  beast::error_code ec;
  stream.shutdown(ec);
  if (ec == asio::error::eof || ec == ssl::error::stream_truncated) {
    ec = {};
  }
  if (ec) {
    std::cerr << "error on stream shutdown " << ec.message() << std::endl;
  }
  stream.lowest_layer().close(ec);
  if (ec) {
    std::cerr << "error on lowest layer close " << ec.message() << std::endl;
  }
}

std::string HttpClient::run_request(http::request<http::string_body> &req,
                                    headers_t _headers) {
  // std::cout << stream.lowest_layer().remote_endpoint().address().to_string()
  // << ' ' << req.target() << '\n';

  req.set(http::field::host, host_bs);

  for (auto &[field, value] : headers) {
    std::cout << "setting " << field << " to " << value << '\n';
    req.set(field, value);
  }
  for (auto &[field, value] : _headers) {
    std::cout << "setting " << field << " to " << value << '\n';
    req.set(field, value);
  }

  http::write(stream, req);

  beast::flat_buffer buffer;
  http::response<http::dynamic_body> res;

  http::read(stream, buffer, res);

  std::string s{};
  for (auto seq : res.body().data()) {
    auto *cbuf = boost::asio::buffer_cast<const char *>(seq);
    s.append(cbuf, boost::asio::buffer_size(seq));
  }

  // std::cout << s << '\n';

  return s;
}

std::string HttpClient::get(std::string_view target, headers_t headers) {
  if (target.length() == 0) {
    target = "/";
  }

  beast::string_view target_bs{target.data(), target.size()};
  http::request<http::string_body> req{http::verb::get, target_bs,
                                       http_version};

  return run_request(req, headers);
}

std::string HttpClient::post(std::string_view target, headers_t headers) {
  if (target.length() == 0) {
    target = "/";
  }

  beast::string_view target_bs{target.data(), target.size()};

  http::request<http::string_body> req{http::verb::post, target_bs,
                                       http_version};

  req.set(http::field::host, host_bs);

  return run_request(req, headers);
}

std::string HttpClient::post(std::string_view target, std::string_view content,
                             headers_t headers) {
  if (target.length() == 0) {
    target = "/";
  }

  beast::string_view target_bs{target.data(), target.size()};

  http::request<http::string_body> req{http::verb::post, target_bs,
                                       http_version, content};

  req.set(http::field::host, host_bs);
  req.prepare_payload();

  return run_request(req, headers);
}
