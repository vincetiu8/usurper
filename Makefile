default: server

server:
	bazel build //src/server
debug-server:
	bazel build //src/server -c dbg
	
cli:
	bazel build //src/cli
	
copy-compile-commands:
	bazel run @hedron_compile_commands//:refresh_all
