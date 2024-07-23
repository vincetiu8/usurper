load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
  name = "models",
  srcs = glob(["*.cc"]),
  hdrs = glob (["*.h"]),
  deps = [
    "//src/reserver/models",
    "//src/utils/db",
  ],
  visibility = ["//src/concierge:__subpackages__", "//src/utils/init:__pkg__", "//tests:__subpackages__"],
)