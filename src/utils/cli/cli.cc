#include <string_view>
#include <vector>

#include "src/utils/cli/cli_args.h"

void process_args(int argc, char* argv[], cli_args& args) {
  for (int i = 0; i < argc; ++i) {
    args[i] = {argv[i]};
  }
}
