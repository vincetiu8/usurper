load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "init",
    srcs = glob(["*.cc"]),
    hdrs = glob(["*.h"]),
    deps = [
      "//src/concierge/models",
      "//src/reserver/models",
      "//src/concierge",
    ],
    visibility = ["//src:__subpackages__", "//tests:__subpackages__"],
)
