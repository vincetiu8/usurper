load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "test_models",
    defines = ["CATCH_CONFIG_MAIN"],
    srcs = glob(["*.cc"]),
    deps = [
      "//src/utils/db",
      "//src/reserver/models",
      "@catch2//:catch2_main",
    ],
)
