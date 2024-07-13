load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "resy",
    srcs = ["resy.cc"],
    hdrs = ["resy.h", "http_client.h"],
    deps = [
      "//src/utils/proxy",
    ],
    visibility = ["//src/booker:__pkg__", "//src/cli/proxies:__pkg__"],
)
