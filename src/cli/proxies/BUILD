load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
  name = "proxies",
  srcs = ["proxy.cc", "resy.cc"],
  hdrs = ["proxy.h", "resy.h"],
  deps = [
    "//src/booker/proxies/resy",
    "//src/utils/cli",
    "//src/utils/proxy",
  ],
  visibility = ["//src/cli:__pkg__"],
)
