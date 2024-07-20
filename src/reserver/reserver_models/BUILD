load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "reserver_models",
    srcs = glob(["*.cc"]),
    hdrs = glob(["*.h"]),
    deps = [
      "//src/utils/geo",
      "//src/utils/time",
      "//src/utils/db",
      "@libpqxx//:libpqxx",
    ],
    visibility = ["//src/reserver:__subpackages__", "//tests:__subpackages__"],
)
