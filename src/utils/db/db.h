#include <pqxx/pqxx>

pqxx::connection *get_db_connection();

pqxx::work get_work();
