#include "src/reserver/proxies/resy/resy_user.h"
#include "src/utils/db/db.h"
#include <optional>
#include <pqxx/pqxx>

void ResyUser::create_table() {
  pqxx::work tx = get_work();

  std::string query = "CREATE TABLE IF NOT EXISTS resy_users ("
                      " user_id INT NOT NULL,"
                      " auth_token TEXT NOT NULL,"
                      " PRIMARY KEY (user_id),"
                      " FOREIGN KEY (user_id) REFERENCES users(id)"
                      ")";

  tx.exec(query);

  tx.commit();
}

void ResyUser::drop_table() {
  pqxx::work tx = get_work();

  std::string query = "DROP TABLE IF EXISTS resy_users";
  tx.exec(query);

  tx.commit();
}

std::optional<ResyUser> ResyUser::get(int user_id) {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "SELECT auth_token FROM resy_users"
                      " WHERE user_id = " +
                      placeholders.get();
  params.append(user_id);

  auto r = tx.query01<std::string>(query, params);

  if (!r.has_value()) {
    return std::nullopt;
  }

  auto [auth_token] = r.value();

  return ResyUser{.user_id = user_id, .auth_token = auth_token};
}

std::vector<ResyUser> ResyUser::get_all() {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "SELECT user_id, auth_token "
                      "FROM resy_users ORDER BY user_id";

  auto r = tx.query<int, std::string>(query, params);

  std::vector<ResyUser> resy_users{};
  for (auto row : r) {
    auto [user_id, auth_token] = row;
    resy_users.push_back(ResyUser{
        .user_id = user_id,
        .auth_token = auth_token,
    });
  }

  return resy_users;
}

void ResyUser::create() {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "INSERT INTO resy_users (user_id, auth_token) VALUES (" +
                      placeholders.get() + ", ";
  params.append(user_id);
  placeholders.next();
  query += placeholders.get() + ")";
  params.append(auth_token);

  tx.exec_params(query, params);

  tx.commit();
}

void ResyUser::update() {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query =
      "UPDATE resy_users SET auth_token = " + placeholders.get();
  params.append(auth_token);
  placeholders.next();
  query += " WHERE user_id = " + placeholders.get();

  params.append(user_id);

  tx.exec_params(query, params);

  tx.commit();
}

void ResyUser::save() {
  std::optional<ResyUser> rsu = ResyUser::get(user_id);

  if (rsu.has_value()) {
    update();
  } else {
    create();
  }
}

void ResyUser::refresh() {
  std::optional<ResyUser> rsu = ResyUser::get(user_id);

  if (rsu.has_value()) {
    *this = rsu.value();
  } else {
    throw std::runtime_error("ResyUser not found");
  }
}

void ResyUser::remove() { remove(user_id); }

void ResyUser::remove(int user_id) {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "DELETE FROM resy_users WHERE "
                      "user_id = " +
                      placeholders.get();

  params.append(user_id);

  tx.exec_params(query, params);

  tx.commit();
}

void ResyUser::remove_all() {
  pqxx::work tx = get_work();

  std::string query = "DELETE FROM resy_users";
  tx.exec(query);

  tx.commit();
}
