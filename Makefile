default: server

server:
	bazel build //src/server --@boost//:beast_use_std_string_view --action_env=CC=/opt/homebrew/opt/llvm/bin/clang
debug-server:
	bazel build //src/server --@boost//:beast_use_std_string_view -c dbg --action_env=CC=/opt/homebrew/opt/llvm/bin/clang
	
cli:
	bazel build //src/cli --@boost//:beast_use_std_string_view --action_env=CC=/opt/homebrew/opt/llvm/bin/clang
	
copy-compile-commands:
	bazel run @hedron_compile_commands//:refresh_all --@boost//:beast_use_std_string_view
