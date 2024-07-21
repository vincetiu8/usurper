#include "src/reserver/models/timeslot.h"
#include "src/utils/db/db.h"
#include <optional>
#include <pqxx/pqxx>
#include <string>
#include <vector>

void Timeslot::create_table() {
  pqxx::work tx = get_work();

  std::string query = "CREATE TABLE IF NOT EXISTS timeslots ("
                      " id SERIAL PRIMARY KEY,"
                      " restaurant_id INT NOT NULL,"
                      " date DATE NOT NULL,"
                      " start_time TIME NOT NULL,"
                      " end_time TIME NOT NULL,"
                      " party_size INT NOT NULL,"
                      " available BOOLEAN NOT NULL,"
                      " FOREIGN KEY (restaurant_id) REFERENCES restaurants(id)"
                      ")";

  tx.exec(query);

  query = "CREATE UNIQUE INDEX IF NOT EXISTS idx_timeslots_rid_date_start ON "
          "timeslots "
          "(restaurant_id, date, start_time)";

  tx.exec(query);

  tx.commit();
}

void Timeslot::drop_table() {
  pqxx::work tx = get_work();

  std::string query = "DROP INDEX IF EXISTS idx_timeslots_rid_date_start";

  tx.exec(query);

  query = "DROP TABLE IF EXISTS timeslots CASCADE";
  tx.exec(query);

  tx.commit();
}

std::optional<Timeslot> Timeslot::get(int id) {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "SELECT restaurant_id, date, start_time, end_time, "
                      "party_size, available FROM timeslots"
                      " WHERE id = " +
                      placeholders.get();
  params.append(id);

  auto r = tx.query01<int, std::string, std::string, std::string, int, bool>(
      query, params);

  if (!r.has_value()) {
    return std::nullopt;
  }

  auto [restaurant_id, date, start_time, end_time, party_size, available] =
      r.value();

  return Timeslot{.id = id,
                  .restaurant_id = restaurant_id,
                  .date = Date(date, "%Y-%m-%d"),
                  .start_time = Time(start_time, "%H:%M:%S"),
                  .end_time = Time(end_time, "%H:%M:%S"),
                  .party_size = party_size,
                  .available = available};
}

std::optional<Timeslot>
Timeslot::get_by_restaurant_id_date_start_time(int restaurant_id, Date date,
                                               Time start_time) {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query =
      "SELECT id, end_time, party_size, available FROM timeslots"
      " WHERE restaurant_id = " +
      placeholders.get();
  params.append(restaurant_id);
  placeholders.next();
  query += " AND date = " + placeholders.get();
  params.append(date.to_yyyy_mm_dd_string());
  placeholders.next();
  query += " AND start_time = " + placeholders.get();
  params.append(start_time.to_hh_mm_ss_string());

  auto r = tx.query01<int, std::string, int, bool>(query, params);

  if (!r.has_value()) {
    return std::nullopt;
  }

  auto [id, end_time, party_size, available] = r.value();

  return Timeslot{.id = id,
                  .restaurant_id = restaurant_id,
                  .date = date,
                  .start_time = start_time,
                  .end_time = Time(end_time, "%H:%M:%S"),
                  .party_size = party_size,
                  .available = available};
}

std::vector<Timeslot> Timeslot::get_by_restaurant_id(int restaurant_id) {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "SELECT id, date, start_time, end_time, party_size, "
                      "available FROM timeslots"
                      " WHERE restaurant_id = " +
                      placeholders.get();
  params.append(restaurant_id);
  query += " ORDER BY date, start_time";

  auto r = tx.query<int, std::string, std::string, std::string, int, bool>(
      query, params);

  std::vector<Timeslot> timeslots;
  for (auto &row : r) {
    auto [id, date, start_time, end_time, party_size, available] = row;
    timeslots.push_back(Timeslot{.id = id,
                                 .restaurant_id = restaurant_id,
                                 .date = Date(date, "%Y-%m-%d"),
                                 .start_time = Time(start_time, "%H:%M:%S"),
                                 .end_time = Time(end_time, "%H:%M:%S"),
                                 .party_size = party_size,
                                 .available = available});
  }

  return timeslots;
}

std::vector<Timeslot> Timeslot::get_all() {
  pqxx::work tx = get_work();

  std::string query = "SELECT id, restaurant_id, date, start_time, end_time, "
                      "party_size, available FROM timeslots"
                      " ORDER BY restaurant_id, date, start_time";
  auto r = tx.query<int, int, std::string, std::string, std::string, int, bool>(
      query);

  std::vector<Timeslot> timeslots;
  for (auto &row : r) {
    auto [id, restaurant_id, date, start_time, end_time, party_size,
          available] = row;
    timeslots.push_back(Timeslot{.id = id,
                                 .restaurant_id = restaurant_id,
                                 .date = Date(date, "%Y-%m-%d"),
                                 .start_time = Time(start_time, "%H:%M:%S"),
                                 .end_time = Time(end_time, "%H:%M:%S"),
                                 .party_size = party_size,
                                 .available = available});
  }

  return timeslots;
}

void Timeslot::create() {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "INSERT INTO timeslots (restaurant_id, date, start_time, "
                      "end_time, party_size, available)"
                      " VALUES (" +
                      placeholders.get() + ", ";
  params.append(restaurant_id);
  placeholders.next();
  query += placeholders.get() + ", ";
  params.append(date.to_yyyy_mm_dd_string());
  placeholders.next();
  query += placeholders.get() + ", ";
  params.append(start_time.to_hh_mm_ss_string());
  placeholders.next();
  query += placeholders.get() + ", ";
  params.append(end_time.to_hh_mm_ss_string());
  placeholders.next();
  query += placeholders.get() + ", ";
  params.append(party_size);
  placeholders.next();
  query += placeholders.get() + ") RETURNING id";
  params.append(available);

  auto r = tx.exec_params1(query, params);

  id = r[0].as<int>();

  tx.commit();
}

void Timeslot::update() {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query =
      "UPDATE timeslots SET restaurant_id = " + placeholders.get() + ", ";
  params.append(restaurant_id);
  placeholders.next();
  query += "date = " + placeholders.get() + ", ";
  params.append(date.to_yyyy_mm_dd_string());
  placeholders.next();
  query += "start_time = " + placeholders.get() + ", ";
  params.append(start_time.to_hh_mm_ss_string());
  placeholders.next();
  query += "end_time = " + placeholders.get() + ", ";
  params.append(end_time.to_hh_mm_ss_string());
  placeholders.next();
  query += "party_size = " + placeholders.get() + ", ";
  params.append(party_size);
  placeholders.next();
  query += "available = " + placeholders.get();
  params.append(available);
  placeholders.next();
  query += " WHERE id = " + placeholders.get();
  params.append(id);

  tx.exec_params(query, params);

  tx.commit();
}

void Timeslot::save() {
  std::optional<Timeslot> timeslot = Timeslot::get(id);

  if (timeslot.has_value()) {
    update();
  } else {
    create();
  }
}

void Timeslot::refresh() {
  std::optional<Timeslot> timeslot = Timeslot::get(id);

  if (timeslot.has_value()) {
    *this = timeslot.value();
  } else {
    throw std::runtime_error("Timeslot does not exist");
  }
}

void Timeslot::remove() { remove(id); }

void Timeslot::remove(int id) {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "DELETE FROM timeslots WHERE id = " + placeholders.get();
  params.append(id);

  tx.exec_params(query, params);

  tx.commit();
}

void Timeslot::remove_all() {
  pqxx::work tx = get_work();

  std::string query = "DELETE FROM timeslots";
  tx.exec(query);

  tx.commit();
}
