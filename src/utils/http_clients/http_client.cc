#include "http_client.h"

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <string>
#include <string_view>

namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

HttpClient::HttpClient(std::string_view host, std::string_view port)
    : host_bs{host.data(), host.size()},
      resolver{ioc},
      stream{ioc},
      http_version{11} {
  auto const results = resolver.resolve(host, port);

  // Make the connection on the IP address we get from a lookup
  stream.connect(results);
}

std::string HttpClient::get(std::string_view target) {
  if (target.length() == 0) {
    target = "/";
  }

  beast::string_view target_bs{target.data(), target.size()};
  http::request<http::string_body> req{http::verb::get, target_bs,
                                       http_version};
  req.set(http::field::host, host_bs);

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

std::string HttpClient::post(std::string_view target, std::string content,
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
