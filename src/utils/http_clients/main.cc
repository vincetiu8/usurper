// //
// // Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
// //
// // Distributed under the Boost Software License, Version 1.0. (See
// accompanying
// // file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
// //
// // Official repository: https://github.com/boostorg/beast
// //

// //------------------------------------------------------------------------------
// //
// // Example: HTTP SSL client, synchronous
// //
// //------------------------------------------------------------------------------

// #include "src/utils/http_clients/root_certificates.h"
// #include <boost/asio/connect.hpp>
// #include <boost/asio/ip/tcp.hpp>
// #include <boost/asio/ssl/error.hpp>
// #include <boost/asio/ssl/stream.hpp>
// #include <boost/beast/core.hpp>
// #include <boost/beast/http.hpp>
// #include <boost/beast/ssl.hpp>
// #include <boost/beast/version.hpp>
// #include <cstdlib>
// #include <iostream>
// #include <string>

// namespace beast = boost::beast; // from <boost/beast.hpp>
// namespace http = beast::http;   // from <boost/beast/http.hpp>
// namespace net = boost::asio;    // from <boost/asio.hpp>
// namespace ssl = net::ssl;       // from <boost/asio/ssl.hpp>
// using tcp = net::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

// // Performs an HTTP GET and prints the response
// int main(int argc, char **argv) {
//   try {
//     auto const host = "api.resy.com";
//     auto const port = "443";
//     auto const target = "/3/details";
//     int version = 11;

//     // The io_context is required for all I/O
//     net::io_context ioc;

//     // The SSL context is required, and holds certificates
//     ssl::context ctx(ssl::context::tlsv12_client);

//     // This holds the root certificate used for verification
//     load_root_certificates(ctx);

//     // Verify the remote server's certificate
//     ctx.set_verify_mode(ssl::verify_peer);

//     // These objects perform our I/O
//     tcp::resolver resolver(ioc);
//     beast::ssl_stream<beast::tcp_stream> stream(ioc, ctx);

//     // Set SNI Hostname (many hosts need this to handshake successfully)
//     if (!SSL_set_tlsext_host_name(stream.native_handle(), host)) {
//       beast::error_code ec{static_cast<int>(::ERR_get_error()),
//                            net::error::get_ssl_category()};
//       throw beast::system_error{ec};
//     }

//     // Look up the domain name
//     auto const results = resolver.resolve(host, port);

//     // Make the connection on the IP address we get from a lookup
//     beast::get_lowest_layer(stream).connect(results);

//     // Perform the SSL handshake
//     stream.handshake(ssl::stream_base::client);

//     std::string content = "{\"commit\":1,\"config_id\":\"rgs://resy/38245/"
//                           "910381/2/2024-07-26/2024-07-26/18:00:00/2/Dining "
//                           "Room\",\"day\":\"2024-07-26\",\"party_size\":\"2\"}";

//     // Set up an HTTP GET request message
//     http::request<http::string_body> req{http::verb::post, target, version,
//                                          content};
//     req.set(http::field::host, host);
//     // req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
//     req.set(http::field::authorization,
//             "ResyAPI api_key=\"VbWk7s3L4KiK5fzlO7JD3Q5EYolJI7n5\"");
//     req.set(http::field::content_type, "application/json");
//     req.prepare_payload();

//     // Send the HTTP request to the remote host
//     http::write(stream, req);

//     // This buffer is used for reading and must be persisted
//     beast::flat_buffer buffer;

//     // Declare a container to hold the response
//     http::response<http::dynamic_body> res;

//     // Receive the HTTP response
//     http::read(stream, buffer, res);

//     // Write the message to standard out
//     std::cout << res << std::endl;

//     // Gracefully close the stream
//     beast::error_code ec;
//     stream.shutdown(ec);
//     if (ec == net::error::eof) {
//       // Rationale:
//       //
//       http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
//       ec = {};
//     }
//     if (ec)
//       throw beast::system_error{ec};

//     // If we get here then the connection is closed gracefully
//   } catch (std::exception const &e) {
//     std::cerr << "Error: " << e.what() << std::endl;
//     return EXIT_FAILURE;
//   }
//   return EXIT_SUCCESS;
// }