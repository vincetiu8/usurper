#include "src/utils/cli/cli.h"
#include "src/cli/db/db.h"
#include "src/cli/proxies/proxy.h"
#include "src/cli/services/concierge.h"
#include "src/cli/services/reserver.h"
#include "src/utils/cli/cli_args.h"
#include "src/utils/init/models.h"
#include "src/utils/init/timer.h"
#include <iostream>
#include <string_view>

int command_handler(cli_args &args) {
  init_models();

  if (args.size() < 2) {
    std::cout << "no command specified" << '\n';
    return 1;
  }

  std::string_view command = args[1];

  if (command == "proxy") {
    return proxy_handler(args);
  }

  if (command == "db") {
    return db_handler(args);
  }

  if (command == "reserver") {
    return reserver_handler(args);
  }

  if (command == "concierge") {
    return concierge_handler(args);
  }

  if (command == "cron") {
    cron();
    return 0;
  }

  std::cout << "unknown command " << command << '\n';
  return 1;
}

/**
 * entrypoint into the cli
 */
int main(int argc, char *argv[]) {
  cli_args args(argc);

  process_args(argc, argv, args);

  return command_handler(args);
}

// constexpr Location defined_location_to_location(DefinedLocation dl) {
//   switch (dl) {
//     case DefinedLocation::new_york:
//       return {40.76464, -73.973};
//     case DefinedLocation::philadelphia:
//       return {40.020446, -75.14894};
//   }
// }
