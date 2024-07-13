load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "proxy",
    srcs = ["http_client.cc"],
    hdrs = ["http_client.h"],
    deps = [
      "@boost//:beast",
      "@boost//:asio",
    ],
    visibility = ["//src:__subpackages__"]
)
