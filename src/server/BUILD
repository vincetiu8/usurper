load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
  name = "server",
  srcs = glob(["*.cc"]),
  deps = [
    "@boost//:beast",
    "@catch2",
  ],
  visibility = ["//visibility:public"]
)

