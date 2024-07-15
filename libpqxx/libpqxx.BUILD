load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

filegroup(
    name = "all_srcs",
    srcs = glob(
        include = ["**"],
        exclude = ["*.bazel"],
    ),
)

cmake(
    name = "libpqxx",
    env = {
      "AR": "",
    },
    cache_entries = {
      "BUILD_DOC": "off",
      "SKIP_BUILD_TEST": "on"
    },
    generate_crosstool_file = False,
    lib_source = ":all_srcs",
    linkopts = [
      "-L/opt/homebrew/opt/libpq/lib",
      "-lpq",
    ],
    visibility = ["//visibility:public"],
    deps = [],
    out_static_libs = [
      "libpqxx.a",
    ]
)
