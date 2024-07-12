#include "src/utils/cli/cli.h"

#include <iostream>
#include <string_view>
#include <vector>

#include "src/cli/proxies/proxy.h"
#include "src/utils/cli/cli_args.h"

int command_handler(cli_args& args) {
  if (args.size() < 2) {
    std::cout << "no command specified" << '\n';
    return 1;
  }

  std::string_view command = args[1];

  if (command == "proxy") {
    return proxy_handler(args);
  }

  std::cout << "unknown command " << command << '\n';
  return 1;
}

/**
 * entrypoint into the cli
 */
int main(int argc, char* argv[]) {
  cli_args args(argc);

  process_args(argc, argv, args);

  return command_handler(args);
}
