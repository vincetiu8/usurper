load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
  name = "db",
  srcs = ["db.cc"],
  hdrs = ["db.h"],
  deps = [
    "//src/utils/cli",
    "//src/utils/db",
    "@libpqxx//:libpqxx",
  ],
  visibility = ["//src/cli:__pkg__"],
)
