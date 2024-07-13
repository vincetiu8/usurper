load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
  name = "proxies",
  srcs = ["proxy.cc", "http_clients.cc", "resy.cc"],
  hdrs = ["proxy.h", "http_clients.h", "resy.h"],
  deps = [
    "//src/booker/proxies/resy",
    "//src/utils/cli",
    "//src/utils/http_clients",
  ],
  visibility = ["//src/cli:__pkg__"],
)
