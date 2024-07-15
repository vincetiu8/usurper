#include <format>
#include <pqxx/pqxx>
#include <string>

#include "src/utils/config/config.h"

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