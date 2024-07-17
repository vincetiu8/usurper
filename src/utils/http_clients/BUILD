load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "http_clients",
    srcs = ["http_client.cc", "http_string_client.cc", "http_json_client.cc"],
    hdrs = ["http_client.h", "http_string_client.h", "http_json_client.h", "root_certificates.h"],
    deps = [
      "@boost//:beast",
      "@boost//:asio_ssl",
      "@boost//:json",
    ],
    visibility = ["//src:__subpackages__"]
)
