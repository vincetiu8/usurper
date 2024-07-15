#include "src/utils/db/db.h"

#include <iostream>
#include <pqxx/pqxx>

#include "src/utils/cli/cli_args.h"

int db_handler(cli_args &args) {
  if (args.size() < 3) {
    std::cout << "no command specified" << '\n';
    return 1;
  }
  if (args.size() < 4) {
    std::cout << "no statement specified" << '\n';
    return 1;
  }

  std::string_view command = args[2];
  std::string_view statement = args[3];

  pqxx::connection conn = get_db_connection();

  if (command == "query") {
    pqxx::work w(conn);
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
    pqxx::work w(conn);
    w.exec(statement.data());
    w.commit();
    return 0;
  }

  std::cout << "unknown db command " << command << '\n';
  return 1;
}
