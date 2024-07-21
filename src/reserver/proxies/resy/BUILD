load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "resy",
    srcs = glob(["*.cc"]),
    hdrs = glob(["*.h"]),
    deps = [
      "//src/utils/http_clients",
      "//src/reserver/models",
      "//src/utils/config",
      "//src/utils/geo",
      "//src/utils/time",
    ],
    visibility = ["//src/reserver:__pkg__", "//src/cli/proxies:__pkg__", "//tests:__subpackages__"],
)
