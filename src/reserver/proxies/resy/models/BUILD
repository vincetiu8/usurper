load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "models",
    srcs = glob(["*.cc"]),
    hdrs = glob(["*.h"]),
    deps = [
      "//src/reserver/reserver_models",
      "//src/utils/config",
      "//src/utils/db",
      "//src/utils/geo",
      "//src/utils/time",
      "@libpqxx//:libpqxx",
    ],
    visibility = ["//src/reserver/proxies/resy:__pkg__", "//tests:__subpackages__"],
)
