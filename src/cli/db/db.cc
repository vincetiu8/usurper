#include "src/utils/db/db.h"
#include "src/utils/cli/cli_args.h"
#include "src/utils/init/models.h"
#include <iostream>
#include <pqxx/pqxx>

int db_handler(cli_args &args) {
  if (args.size() < 3) {
    std::cout << "no command specified" << '\n';
    return 1;
  }

  std::string_view command = args[2];

  if (command == "query") {
    if (args.size() < 4) {
      std::cout << "no statement specified" << '\n';
      return 1;
    }

    std::string_view statement = args[3];

    pqxx::work w = get_work();
    pqxx::result r = w.exec(statement.data());
    w.commit();
    for (auto row : r) {
      for (auto field : row) {
        std::cout << field.c_str() << '\t';
      }
      std::cout << '\n';
    }
    return 0;
  }

  if (command == "execute") {
    if (args.size() < 4) {
      std::cout << "no statement specified" << '\n';
      return 1;
    }

    std::string_view statement = args[3];

    pqxx::work w = get_work();
    w.exec(statement.data());
    w.commit();
    return 0;
  }

  if (command == "reset") {
    reset_models();
    return 0;
  }

  std::cout << "unknown db command " << command << '\n';
  return 1;
}
