#include "src/reserver/reserver_models/reservation_service_user.h"
#include "src/reserver/reserver_models/reservation_service_code.h"
#include "src/utils/db/db.h"
#include <optional>
#include <pqxx/pqxx>

void ReservationServiceUser::create_table() {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "SELECT EXISTS ("
                      " SELECT FROM information_schema.tables"
                      " WHERE table_name = " +
                      placeholders.get();
  query += ")";
  params.append("reservation_service_users");

  auto r = tx.query1<bool>(query, params);

  if (std::get<0>(r)) {
    tx.commit();
    return;
  }

  query = "CREATE TABLE IF NOT EXISTS reservation_service_users ("
          " user_id INT NOT NULL,"
          " reservation_service_code TEXT NOT NULL,"
          " auth_token TEXT NOT NULL,"
          " PRIMARY KEY (user_id, reservation_service_code),"
          " FOREIGN KEY (user_id) REFERENCES users(id)"
          ")";

  tx.exec(query);

  tx.commit();
}

void ReservationServiceUser::drop_table() {
  pqxx::work tx = get_work();

  std::string query = "DROP TABLE IF EXISTS reservation_service_users";
  tx.exec(query);

  tx.commit();
}

std::optional<ReservationServiceUser>
ReservationServiceUser::get(ReservationServiceCode rsc, int user_id) {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "SELECT auth_token FROM reservation_service_users"
                      " WHERE user_id = " +
                      placeholders.get();
  params.append(user_id);
  placeholders.next();
  query += " AND reservation_service_code = " + placeholders.get();
  params.append(reservation_service_code_to_string(rsc));

  auto r = tx.query01<std::string>(query, params);

  if (!r.has_value()) {
    return std::nullopt;
  }

  auto [auth_token] = r.value();

  return ReservationServiceUser{.user_id = user_id,
                                .reservation_service_code = rsc,
                                .auth_token = auth_token};
}

std::vector<ReservationServiceUser> ReservationServiceUser::get_all() {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "SELECT user_id, reservation_service_code, auth_token "
                      "FROM reservation_service_users ORDER BY user_id";

  auto r = tx.query<int, std::string, std::string>(query, params);

  std::vector<ReservationServiceUser> reservation_service_users{};
  for (auto row : r) {
    auto [user_id, reservation_service_code_string, auth_token] = row;
    ReservationServiceCode rsc =
        string_to_reservation_service_code(reservation_service_code_string);
    reservation_service_users.push_back(ReservationServiceUser{
        .user_id = user_id,
        .reservation_service_code = rsc,
        .auth_token = auth_token,
    });
  }

  return reservation_service_users;
}

void ReservationServiceUser::create() {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "INSERT INTO reservation_service_users (user_id, "
                      "reservation_service_code, auth_token) VALUES (" +
                      placeholders.get() + ", ";
  params.append(user_id);
  placeholders.next();
  query += placeholders.get() + ", ";
  params.append(reservation_service_code_to_string(reservation_service_code));
  placeholders.next();
  query += placeholders.get() + ")";
  params.append(auth_token);

  tx.exec_params(query, params);

  tx.commit();
}

void ReservationServiceUser::update() {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query =
      "UPDATE reservation_service_users SET auth_token = " + placeholders.get();
  params.append(auth_token);
  placeholders.next();
  query += " WHERE user_id = " + placeholders.get();

  params.append(user_id);
  placeholders.next();
  query += " AND reservation_service_code = " + placeholders.get();

  params.append(reservation_service_code_to_string(reservation_service_code));
  tx.exec_params(query, params);

  tx.commit();
}

void ReservationServiceUser::save() {
  std::optional<ReservationServiceUser> rsu =
      ReservationServiceUser::get(reservation_service_code, user_id);

  if (rsu.has_value()) {
    update();
  } else {
    create();
  }
}

void ReservationServiceUser::refresh() {
  std::optional<ReservationServiceUser> rsu =
      ReservationServiceUser::get(reservation_service_code, user_id);

  if (rsu.has_value()) {
    *this = rsu.value();
  } else {
    throw std::runtime_error("ReservationServiceUser not found");
  }
}

void ReservationServiceUser::remove() {
  remove(reservation_service_code, user_id);
}

void ReservationServiceUser::remove(ReservationServiceCode rsc, int user_id) {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "DELETE FROM reservation_service_users WHERE "
                      "user_id = " +
                      placeholders.get();

  params.append(user_id);
  placeholders.next();
  query += " AND reservation_service_code = " + placeholders.get();

  params.append(reservation_service_code_to_string(rsc));
  tx.exec_params(query, params);

  tx.commit();
}

void ReservationServiceUser::remove_by_user_id(int user_id) {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "DELETE FROM reservation_service_users WHERE user_id = " +
                      placeholders.get();
  params.append(user_id);

  tx.exec_params(query, params);

  tx.commit();
}

void ReservationServiceUser::remove_all() {
  pqxx::work tx = get_work();

  std::string query = "DELETE FROM reservation_service_users";
  tx.exec(query);

  tx.commit();
}
