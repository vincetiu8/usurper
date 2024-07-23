load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "models",
    srcs = glob(["*.cc"]),
    hdrs = glob(["*.h"]),
    deps = [
      "//src/utils/geo",
      "//src/utils/time",
      "//src/utils/db",
      "@libpqxx//:libpqxx",
      "//src/utils/string",
    ],
    visibility = [
      "//src/reserver:__subpackages__",
      "//src/cli/services:__pkg__",
      "//src/utils/init:__pkg__",
      "//src/concierge:__subpackages__",
      "//tests:__subpackages__",
    ],
)
