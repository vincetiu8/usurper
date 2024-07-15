load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "db",
    hdrs = ["db.h"],
    srcs = ["db.cc"],
    deps = [
      "//src/utils/config",
      "@libpqxx//:libpqxx",
    ],
    visibility = ["//src:__subpackages__"],
)
