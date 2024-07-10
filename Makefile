default: build run
build: 
	bazel build //src:main --@boost//:beast_use_std_string_view
run: 
	bazel-bin/src/main
copy-compile-commands:
	bazel run @hedron_compile_commands//:refresh_all --@boost//:beast_use_std_string_view