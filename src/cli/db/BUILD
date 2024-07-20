load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
  name = "db",
  srcs = glob(["*.cc"]),
  hdrs = glob(["*.h"]),
  deps = [
    "//src/utils/cli",
    "//src/utils/db",
    "@libpqxx//:libpqxx",
  ],
  visibility = ["//src/cli:__pkg__"],
)
