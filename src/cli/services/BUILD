load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
  name = "services",
  srcs = glob(["*.cc"]),
  hdrs = glob(["*.h"]),
  deps = [
    "//src/reserver",
    "//src/concierge",
    "//src/utils/cli",
    "//src/utils/http_clients",
    "//src/utils/config",
  ],
  visibility = ["//src/cli:__pkg__"],
)
