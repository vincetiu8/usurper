load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "test_time",
    defines = ["CATCH_CONFIG_MAIN"],
    srcs = glob(["*.cc"]),
    deps = [
      "//src/utils/time",
      "@catch2//:catch2_main",
    ],
)
