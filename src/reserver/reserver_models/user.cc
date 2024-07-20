#include "src/reserver/reserver_models/user.h"
#include "src/utils/db/db.h"
#include <optional>
#include <pqxx/pqxx>
#include <string>

void User::create_table() {
  pqxx::work tx = get_work();

  std::string query = "CREATE TABLE IF NOT EXISTS users ("
                      " id SERIAL PRIMARY KEY,"
                      " name TEXT NOT NULL"
                      ")";

  tx.exec(query);

  tx.commit();
}

void User::drop_table() {
  pqxx::work tx = get_work();

  std::string query = "DROP TABLE IF EXISTS users";
  tx.exec(query);

  tx.commit();
}

std::optional<User> User::get(int id) {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "SELECT name FROM users WHERE id = " + placeholders.get();
  params.append(id);

  auto r = tx.query01<std::string>(query, params);

  if (!r.has_value()) {
    return std::nullopt;
  }

  auto [name] = r.value();

  return User{.id = id, .name = name};
}

std::vector<User> User::get_all() {
  pqxx::work tx = get_work();

  std::string query = "SELECT id, name FROM users ORDER BY id";
  auto r = tx.query<int, std::string>(query);

  std::vector<User> users;
  for (auto row : r) {
    auto [id, name] = row;
    users.push_back(User{.id = id, .name = name});
  }

  return users;
}

void User::create() {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "INSERT INTO users (name) VALUES (" + placeholders.get() +
                      ") RETURNING id";
  params.append(name);

  auto r = tx.exec_params1(query, params);

  this->id = r[0].as<int>();

  tx.commit();
}

void User::update() {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "UPDATE users SET name = " + placeholders.get();
  params.append(name);
  placeholders.next();
  query += " WHERE id = " + placeholders.get();
  params.append(id);

  tx.exec_params(query, params);

  tx.commit();
}

void User::save() {
  std::optional<User> user = User::get(id);

  if (user.has_value()) {
    update();
  } else {
    create();
  }
}

void User::refresh() {
  std::optional<User> user = User::get(id);

  if (user.has_value()) {
    *this = user.value();
  } else {
    throw std::runtime_error("User does not exist");
  }
}

void User::remove() { remove(id); }

void User::remove(int id) {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "DELETE FROM users WHERE id = " + placeholders.get();
  params.append(id);

  tx.exec_params(query, params);

  tx.commit();
}

void User::remove_all() {
  pqxx::work tx = get_work();

  std::string query = "DELETE FROM users";
  tx.exec(query);

  tx.commit();
}
