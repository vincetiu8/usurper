load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "db",
    srcs = ["db.cc"],
    hdrs = ["db.h"],
    deps = [
      "@boost//:asio",
      "//src/utils/config",
      "@libpqxx//:libpqxx",
    ],
    visibility = ["//src:__subpackages__"],
)
