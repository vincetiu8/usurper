load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "reserver_models",
    hdrs = ["models.h", "reservation_proxy.h"],
    deps = [
      "//src/utils/geo",
      "//src/utils/time",
    ],
    visibility = ["//src/reserver:__subpackages__"],
)
