default: server

server: debug-server
	# bazel build //src/server
debug-server:
	bazel build -c dbg //src/server 
	
cli: debug-cli
	# bazel build //src/cli
debug-cli:
	bazel build -c dbg //src/cli 

test: debug-test
debug-test:
	bazel test --local_test_jobs=1 --test_output=all -c dbg //tests/... 
	
copy-compile-commands:
	bazel run @hedron_compile_commands//:refresh_all
