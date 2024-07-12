load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
  name = "cli",
  srcs = ["cli.cc"],
  deps = [
    "//src/cli/proxies",
    "//src/utils/cli",
  ],
  visibility = ["//visibility:public"]
)
