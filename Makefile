default: server

server: debug-server
	# bazel build //src/server
debug-server:
	bazel build //src/server -c dbg
	
cli: debug-cli
	# bazel build //src/cli
debug-cli:
	bazel build //src/cli -c dbg
	
copy-compile-commands:
	bazel run @hedron_compile_commands//:refresh_all
