load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def libpqxx_repository():
  http_archive(
    name = "libpqxx",
    url = "https://github.com/jtv/libpqxx/archive/refs/tags/7.9.1.tar.gz",
    build_file = Label("//libpqxx:libpqxx.BUILD"),
    strip_prefix = "libpqxx-7.9.1",
  )

def _non_module_dependencies_impl(_ctx):
  libpqxx_repository()

non_module_dependencies = module_extension(
    implementation = _non_module_dependencies_impl,
)
