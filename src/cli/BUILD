load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
  name = "cli",
  srcs = glob(["*.cc"]),
  deps = [
    "//src/cli/proxies",
    "//src/cli/services",
    "//src/utils/cli",
    "//src/cli/db",
    "//src/utils/init",
  ],
  visibility = ["//visibility:public"]
)
