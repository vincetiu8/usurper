#include "src/utils/config/config.h"
#include <pqxx/pqxx>
#include <string>

pqxx::connection get_db_connection() {
  std::string conn_str = "postgresql://";
  conn_str += POSTGRES_USER;
  conn_str += ":";
  conn_str += POSTGRES_PASSWORD;
  conn_str += "@";
  conn_str += POSTGRES_HOST;
  conn_str += "/";
  conn_str += POSTGRES_DATABASE;

  return pqxx::connection(conn_str);
}

pqxx::connection conn = get_db_connection();

pqxx::work get_transaction() { return pqxx::work(conn); }
