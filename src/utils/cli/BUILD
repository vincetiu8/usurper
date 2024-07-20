load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "cli",
    srcs = glob(["*.cc"]),
    hdrs = glob(["*.h"]),
    visibility = ["//src:__subpackages__", "//tests:__subpackages__"],
)
