load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
  name = "concierge",
  srcs = glob(["*.cc"]),
  hdrs = glob (["*.h"]),
  deps = [
    "//src/reserver/models",
    "//src/reserver",
    "//src/concierge/models",
    "//src/utils/string",
  ],
  visibility = ["//src/cli/services:__pkg__", "//src/utils/init:__pkg__"],
)
