load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "test_resy",
    defines = ["CATCH_CONFIG_MAIN"],
    srcs = glob(["*.cc"]),
    deps = [
      "//src/utils/db",
      "//src/reserver/reserver_models",
      "//src/reserver/proxies/resy",
      "@catch2//:catch2_main",
    ],
)
