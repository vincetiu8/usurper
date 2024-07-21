load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "reserver",
    srcs = glob(["*.cc"]),
    hdrs = glob(["*.h"]),
    deps = [
        "//src/reserver/proxies/resy",
        "//src/reserver/models"
    ],
    visibility = ["//src:__pkg__", "//tests:__subpackages__"],
)
