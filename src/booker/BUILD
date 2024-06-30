load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "booker",
    srcs = ["booker.cc"],
    hdrs = ["booker.h"],
    deps = [
        "//resy:resy",
    ],
    visibility = ["//main:__pkg__"],
)