load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "db",
    hdrs = glob(["*.h"]),
    srcs = glob(["*.cc"]),
    deps = [
      "//src/utils/config",
      "@libpqxx//:libpqxx",
    ],
    visibility = ["//src:__subpackages__", "//tests:__subpackages__"],
)
