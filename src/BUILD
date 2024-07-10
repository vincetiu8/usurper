load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [
        "//src/booker:booker",
        "//src/concierge:concierge",
        "@boost//:beast",
        "@catch2"
    ],
)
