#include "src/utils/cli/cli_args.h"
#include <string_view>
#include <vector>

void process_args(int argc, char *argv[], cli_args &args) {
  for (int i = 0; i < argc; ++i) {
    args[i] = {argv[i]};
  }
}
