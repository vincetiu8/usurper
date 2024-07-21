#include "src/reserver/reserver_models/booking.h"
#include "src/utils/db/db.h"
#include <optional>
#include <pqxx/pqxx>
#include <vector>

void Booking::create_table() {
  pqxx::work tx = get_work();

  std::string query = "CREATE TABLE IF NOT EXISTS bookings ("
                      "user_id INTEGER NOT NULL,"
                      "timeslot_id INTEGER NOT NULL,"
                      "PRIMARY KEY (user_id, timeslot_id),"
                      "FOREIGN KEY (user_id) REFERENCES users(id),"
                      "FOREIGN KEY (timeslot_id) REFERENCES timeslots(id)"
                      ");";

  tx.exec(query);

  tx.commit();
}

void Booking::drop_table() {
  pqxx::work tx = get_work();

  std::string query = "DROP TABLE IF EXISTS bookings CASCADE";
  tx.exec(query);

  tx.commit();
}

std::optional<Booking> Booking::get(int user_id, int timeslot_id) {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "SELECT FROM bookings"
                      " WHERE user_id = " +
                      placeholders.get();
  params.append(user_id);
  placeholders.next();
  query += " AND timeslot_id = " + placeholders.get();
  params.append(timeslot_id);

  auto r = tx.query01<>(query, params);

  if (!r.has_value()) {
    return std::nullopt;
  }

  return Booking{.user_id = user_id, .timeslot_id = timeslot_id};
}

std::vector<Booking> Booking::get_by_user_id(int user_id) {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "SELECT timeslot_id FROM bookings"
                      " WHERE user_id = " +
                      placeholders.get();
  params.append(user_id);

  auto r = tx.query<int>(query, params);

  std::vector<Booking> bookings;
  for (auto row : r) {
    auto [timeslot_id] = row;
    bookings.push_back(Booking{.user_id = user_id, .timeslot_id = timeslot_id});
  }

  return bookings;
}

std::vector<Booking> Booking::get_all() {
  pqxx::work tx = get_work();

  std::string query = "SELECT user_id, timeslot_id FROM bookings";
  auto r = tx.query<int, int>(query);

  std::vector<Booking> bookings;
  for (auto row : r) {
    auto [user_id, timeslot_id] = row;
    bookings.push_back(Booking{.user_id = user_id, .timeslot_id = timeslot_id});
  }

  return bookings;
}

void Booking::create() {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "INSERT INTO bookings (user_id, timeslot_id) VALUES (" +
                      placeholders.get();
  params.append(user_id);
  placeholders.next();
  query += ", " + placeholders.get() + ")";
  params.append(timeslot_id);

  tx.exec_params(query, params);

  tx.commit();
}

void Booking::remove() { remove(user_id, timeslot_id); }

void Booking::remove(int user_id, int timeslot_id) {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query =
      "DELETE FROM bookings WHERE user_id = " + placeholders.get();
  params.append(user_id);
  placeholders.next();
  query += " AND timeslot_id = " + placeholders.get();
  params.append(timeslot_id);

  tx.exec_params(query, params);

  tx.commit();
}

void Booking::remove_by_user_id(int user_id) {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query =
      "DELETE FROM bookings WHERE user_id = " + placeholders.get();
  params.append(user_id);

  tx.exec_params(query, params);

  tx.commit();
}

void Booking::remove_all() {
  pqxx::work tx = get_work();

  std::string query = "DELETE FROM bookings";
  tx.exec(query);

  tx.commit();
}
