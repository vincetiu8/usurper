load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "reserver",
    srcs = ["reserver.cc"],
    hdrs = ["reserver.h"],
    deps = [
        "//src/reserver/proxies/resy:resy",
    ],
    visibility = ["//src:__pkg__"],
)
