load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
  name = "server",
  srcs = ["server.cc"],
  deps = [
    "@boost//:beast",
    "@catch2"
  ],
  visibility = ["//visibility:public"]
)

