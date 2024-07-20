load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "http_clients",
    srcs = glob(["*.cc"]),
    hdrs = glob(["*.h"]),
    deps = [
      "@boost//:beast",
      "@boost//:asio_ssl",
      "@boost//:json",
    ],
    visibility = ["//src:__subpackages__", "//tests:__subpackages__"],
)

# cc_binary(
#     name = "http_clients_main",
#     srcs = ["main.cc"],
#     deps = [
#         "@boost//:beast",
#         "@boost//:asio_ssl",
#         "@boost//:json",
#         ":http_clients",
#     ],
# )
