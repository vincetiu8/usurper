load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "reserver_models_test",
    defines = ["CATCH_CONFIG_MAIN"],
    srcs = glob(["*.cc"]),
    deps = [
      "//src/utils/db",
      "//src/reserver/reserver_models",
      "@catch2//:catch2_main",
    ],
)
