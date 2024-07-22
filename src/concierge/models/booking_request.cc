#include "src/concierge/models/booking_request.h"
#include "src/utils/db/db.h"
#include <pqxx/pqxx>

void BookingRequest::create_table() {
  pqxx::work tx = get_work();

  std::string query = "CREATE TABLE IF NOT EXISTS booking_requests ("
                      "id SERIAL PRIMARY KEY,"
                      "user_id INTEGER NOT NULL,"
                      "restaurant_id INTEGER NOT NULL,"
                      "date TEXT NOT NULL,"
                      "earliest_time TEXT NOT NULL,"
                      "latest_time TEXT NOT NULL,"
                      "party_size INTEGER NOT NULL,"
                      "FOREIGN KEY (user_id) REFERENCES users(id),"
                      "FOREIGN KEY (restaurant_id) REFERENCES restaurants(id)"
                      ");";

  tx.exec(query);

  tx.commit();
}

void BookingRequest::drop_table() {
  pqxx::work tx = get_work();

  std::string query = "DROP TABLE IF EXISTS booking_requests CASCADE";
  tx.exec(query);

  tx.commit();
}

std::optional<BookingRequest> BookingRequest::get(int id) {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "SELECT user_id, restaurant_id, date, earliest_time, "
                      "latest_time, party_size FROM booking_requests"
                      " WHERE id = " +
                      placeholders.get();
  params.append(id);

  auto r = tx.query01<int, int, std::string, std::string, std::string, int>(
      query, params);

  if (!r.has_value()) {
    return std::nullopt;
  }

  auto [user_id, restaurant_id, date, earliest_time, latest_time, party_size] =
      r.value();

  return BookingRequest{.id = id,
                        .user_id = user_id,
                        .restaurant_id = restaurant_id,
                        .date = Date(date, "%Y-%m-%d"),
                        .earliest_time = Time(earliest_time, "%H:%M:%S"),
                        .latest_time = Time(latest_time, "%H:%M:%S"),
                        .party_size = party_size};
}

std::vector<BookingRequest> BookingRequest::get_by_user_id(int user_id) {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "SELECT id, restaurant_id, date, earliest_time, "
                      "latest_time, party_size FROM booking_requests"
                      " WHERE user_id = " +
                      placeholders.get();
  params.append(user_id);

  auto r = tx.query<int, int, std::string, std::string, std::string, int>(
      query, params);

  std::vector<BookingRequest> booking_requests;
  for (auto row : r) {
    auto [id, restaurant_id, date, earliest_time, latest_time, party_size] =
        row;
    booking_requests.push_back(
        BookingRequest{.id = id,
                       .user_id = user_id,
                       .restaurant_id = restaurant_id,
                       .date = Date(date, "%Y-%m-%d"),
                       .earliest_time = Time(earliest_time, "%H:%M:%S"),
                       .latest_time = Time(latest_time, "%H:%M:%S"),
                       .party_size = party_size});
  }

  return booking_requests;
}

std::vector<BookingRequest> BookingRequest::get_all() {
  pqxx::work tx = get_work();

  std::string query =
      "SELECT id, user_id, restaurant_id, date, earliest_time, latest_time, "
      "party_size FROM booking_requests ORDER BY id";
  auto r = tx.query<int, int, int, std::string, std::string, std::string, int>(
      query);

  std::vector<BookingRequest> booking_requests;
  for (auto row : r) {
    auto [id, user_id, restaurant_id, date, earliest_time, latest_time,
          party_size] = row;
    booking_requests.push_back(
        BookingRequest{.id = id,
                       .user_id = user_id,
                       .restaurant_id = restaurant_id,
                       .date = Date(date, "%Y-%m-%d"),
                       .earliest_time = Time(earliest_time, "%H:%M:%S"),
                       .latest_time = Time(latest_time, "%H:%M:%S"),
                       .party_size = party_size});
  }

  return booking_requests;
}

void BookingRequest::create() {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "INSERT INTO booking_requests (user_id, restaurant_id, "
                      "date, earliest_time, latest_time, party_size) VALUES (" +
                      placeholders.get();
  params.append(user_id);
  placeholders.next();
  query += ", " + placeholders.get();
  params.append(restaurant_id);
  placeholders.next();
  query += ", " + placeholders.get();
  params.append(date.to_yyyy_mm_dd_string());
  placeholders.next();
  query += ", " + placeholders.get();
  params.append(earliest_time.to_hh_mm_ss_string());
  placeholders.next();
  query += ", " + placeholders.get();
  params.append(latest_time.to_hh_mm_ss_string());
  placeholders.next();
  query += ", " + placeholders.get();
  params.append(party_size);
  query += ") RETURNING id";

  auto r = tx.exec_params1(query, params);

  this->id = r[0].as<int>();

  tx.commit();
}

void BookingRequest::update() {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query =
      "UPDATE booking_requests SET user_id = " + placeholders.get();
  params.append(user_id);
  placeholders.next();
  query += ", restaurant_id = " + placeholders.get();
  params.append(restaurant_id);
  placeholders.next();
  query += ", date = " + placeholders.get();
  params.append(date.to_yyyy_mm_dd_string());
  placeholders.next();
  query += ", earliest_time = " + placeholders.get();
  params.append(earliest_time.to_hh_mm_ss_string());
  placeholders.next();
  query += ", latest_time = " + placeholders.get();
  params.append(latest_time.to_hh_mm_ss_string());
  placeholders.next();
  query += ", party_size = " + placeholders.get();
  params.append(party_size);
  placeholders.next();
  query += " WHERE id = " + placeholders.get();
  params.append(id);

  tx.exec_params(query, params);

  tx.commit();
}

void BookingRequest::save() {
  std::optional<BookingRequest> booking_request = get(id);

  if (booking_request.has_value()) {
    update();
  } else {
    create();
  }
}

void BookingRequest::refresh() {
  std::optional<BookingRequest> booking_request = get(id);

  if (booking_request.has_value()) {
    *this = booking_request.value();
  } else {
    throw std::runtime_error("BookingRequest not found");
  }
}

void BookingRequest::remove() { remove(id); }

void BookingRequest::remove(int id) {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query =
      "DELETE FROM booking_requests WHERE id = " + placeholders.get();
  params.append(id);

  tx.exec_params(query, params);

  tx.commit();
}

void BookingRequest::remove_by_user_id(int user_id) {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query =
      "DELETE FROM booking_requests WHERE user_id = " + placeholders.get();
  params.append(user_id);

  tx.exec_params(query, params);

  tx.commit();
}

void BookingRequest::remove_all() {
  pqxx::work tx = get_work();

  std::string query = "DELETE FROM booking_requests";
  tx.exec(query);

  tx.commit();
}
