#include <iostream>
#include <pqxx/pqxx>

#include "src/utils/cli/cli_args.h"

int db_handler(cli_args &args) {
  pqxx::connection conn{"postgresql://user:password@localhost/usurper"};

  std::cout << "connected to " << conn.dbname() << '\n';
  return 0;
}
