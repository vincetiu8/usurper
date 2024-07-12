load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
  name = "proxies",
  srcs = ["proxy.cc", "resy.cc"],
  hdrs = ["proxy.h", "resy.h"],
  deps = [
    "//src/utils/cli",
  ],
  visibility = ["//src/cli:__pkg__"],
)
