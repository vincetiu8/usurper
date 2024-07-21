#include "src/reserver/models/booking.h"
#include "src/utils/db/db.h"
#include <optional>
#include <pqxx/pqxx>
#include <vector>

void Booking::create_table() {
  pqxx::work tx = get_work();

  std::string query = "CREATE TABLE IF NOT EXISTS bookings ("
                      "user_id INTEGER NOT NULL,"
                      "timeslot_id INTEGER NOT NULL,"
                      "resy_token TEXT,"
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
  std::string query = "SELECT resy_token FROM bookings"
                      " WHERE user_id = " +
                      placeholders.get();
  params.append(user_id);
  placeholders.next();
  query += " AND timeslot_id = " + placeholders.get();
  params.append(timeslot_id);

  auto r = tx.query01<std::optional<std::string>>(query, params);

  if (!r.has_value()) {
    return std::nullopt;
  }

  auto [resy_token] = r.value();

  return Booking{
      .user_id = user_id, .timeslot_id = timeslot_id, .resy_token = resy_token};
}

std::vector<Booking> Booking::get_by_user_id(int user_id) {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "SELECT timeslot_id, resy_token FROM bookings"
                      " WHERE user_id = " +
                      placeholders.get();
  params.append(user_id);

  auto r = tx.query<int, std::optional<std::string>>(query, params);

  std::vector<Booking> bookings;
  for (auto row : r) {
    auto [timeslot_id, resy_token] = row;
    bookings.push_back(Booking{.user_id = user_id,
                               .timeslot_id = timeslot_id,
                               .resy_token = resy_token});
  }

  return bookings;
}

std::vector<Booking> Booking::get_all() {
  pqxx::work tx = get_work();

  std::string query = "SELECT user_id, timeslot_id, resy_token FROM bookings";
  auto r = tx.query<int, int, std::optional<std::string>>(query);

  std::vector<Booking> bookings;
  for (auto row : r) {
    auto [user_id, timeslot_id, resy_token] = row;
    bookings.push_back(Booking{.user_id = user_id,
                               .timeslot_id = timeslot_id,
                               .resy_token = resy_token});
  }

  return bookings;
}

void Booking::create() {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query =
      "INSERT INTO bookings (user_id, timeslot_id, resy_token) VALUES (" +
      placeholders.get();
  params.append(user_id);
  placeholders.next();
  query += ", " + placeholders.get();
  params.append(timeslot_id);
  placeholders.next();
  query += ", " + placeholders.get();
  params.append(resy_token);
  query += ")";

  tx.exec_params(query, params);

  tx.commit();
}

void Booking::update() {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "UPDATE bookings SET resy_token = " + placeholders.get();
  params.append(resy_token);
  placeholders.next();
  query += " WHERE user_id = " + placeholders.get();
  params.append(user_id);
  placeholders.next();
  query += " AND timeslot_id = " + placeholders.get();
  params.append(timeslot_id);

  tx.exec_params(query, params);

  tx.commit();
}

void Booking::save() {
  std::optional<Booking> booking = get(user_id, timeslot_id);

  if (booking.has_value()) {
    update();
  } else {
    create();
  }
}

void Booking::refresh() {
  std::optional<Booking> booking = get(user_id, timeslot_id);

  if (booking.has_value()) {
    *this = booking.value();
  } else {
    throw std::runtime_error("Booking does not exist");
  }
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
