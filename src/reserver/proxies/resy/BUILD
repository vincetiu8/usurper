load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "resy",
    srcs = ["resy.cc"],
    hdrs = ["resy.h", "http_client.h"],
    deps = [
      "//src/utils/http_clients",
      "//src/reserver/reserver_models",
      "//src/utils/config",
    ],
    visibility = ["//src/reserver:__pkg__", "//src/cli/proxies:__pkg__"],
)
