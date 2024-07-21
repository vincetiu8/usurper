#include "src/reserver/models/restaurant.h"
#include "src/utils/db/db.h"
#include "src/utils/string/string.h"
#include <optional>
#include <pqxx/pqxx>

void Restaurant::create_table() {
  pqxx::work tx = get_work();

  std::string query = "CREATE TABLE IF NOT EXISTS restaurants ("
                      " id SERIAL PRIMARY KEY,"
                      " name TEXT NOT NULL,"
                      " reservation_service_code TEXT,"
                      " resy_id INT"
                      ")";

  tx.exec(query);

  query =
      "CREATE INDEX IF NOT EXISTS idx_restaurants_name ON restaurants (name)";

  tx.exec(query);

  query = "CREATE UNIQUE INDEX IF NOT EXISTS idx_restaurants_resy_id ON "
          "restaurants (resy_id)";

  tx.exec(query);

  tx.commit();
}

void Restaurant::drop_table() {
  pqxx::work tx = get_work();

  std::string query = "DROP TABLE IF EXISTS restaurants CASCADE";
  tx.exec(query);

  tx.commit();
}

std::optional<Restaurant> Restaurant::get(int id) {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query =
      "SELECT name, reservation_service_code, resy_id FROM restaurants"
      " WHERE id = " +
      placeholders.get();
  params.append(id);

  auto r =
      tx.query01<std::string, std::string, std::optional<int>>(query, params);

  if (!r.has_value()) {
    return std::nullopt;
  }

  auto [name, rsc, resy_id] = r.value();

  Restaurant restaurant{
      .id = id,
      .name = name,
      .rsc = string_to_reservation_service_code(rsc),
      .resy_id = resy_id,
  };

  return restaurant;
}

std::vector<Restaurant> Restaurant::get_by_name(std::string &name) {
  to_lowercase(name);

  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query =
      "SELECT id, name, reservation_service_code, resy_id FROM restaurants"
      " WHERE name LIKE " +
      placeholders.get();
  params.append('%' + name + '%');
  query += " ORDER BY id";

  auto r = tx.query<int, std::string, std::string, std::optional<int>>(query,
                                                                       params);

  std::vector<Restaurant> restaurants;
  for (const auto &row : r) {
    auto [id, name, rsc, resy_id] = row;
    Restaurant restaurant{
        .id = id,
        .name = name,
        .rsc = string_to_reservation_service_code(rsc),
        .resy_id = resy_id,
    };
    restaurants.push_back(restaurant);
  }

  return restaurants;
}

std::optional<Restaurant> Restaurant::get_by_resy_id(int resy_id) {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query =
      "SELECT id, name, reservation_service_code FROM restaurants"
      " WHERE resy_id = " +
      placeholders.get();
  params.append(resy_id);

  auto r = tx.query01<int, std::string, std::string>(query, params);

  if (!r.has_value()) {
    return std::nullopt;
  }

  auto [id, name, rsc] = r.value();

  Restaurant restaurant{
      .id = id,
      .name = name,
      .rsc = string_to_reservation_service_code(rsc),
      .resy_id = resy_id,
  };

  return restaurant;
}

std::vector<Restaurant> Restaurant::get_all() {
  pqxx::work tx = get_work();

  std::string query = "SELECT id, name, reservation_service_code, resy_id FROM "
                      "restaurants ORDER BY id";
  auto r = tx.query<int, std::string, std::string, std::optional<int>>(query);

  std::vector<Restaurant> restaurants;
  for (const auto &row : r) {
    auto [id, name, rsc, resy_id] = row;
    Restaurant restaurant{
        .id = id,
        .name = name,
        .rsc = string_to_reservation_service_code(rsc),
        .resy_id = resy_id,
    };
    restaurants.push_back(restaurant);
  }

  return restaurants;
}

void Restaurant::create() {
  to_lowercase(name);

  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "INSERT INTO restaurants (name, "
                      "reservation_service_code, resy_id) VALUES (" +
                      placeholders.get();
  params.append(name);
  placeholders.next();
  query += ", " + placeholders.get();
  params.append(reservation_service_code_to_string(rsc));
  placeholders.next();
  query += ", " + placeholders.get();
  params.append(resy_id);
  query += ") RETURNING id";

  auto r = tx.exec_params1(query, params);

  id = r[0].as<int>();

  tx.commit();
}

void Restaurant::update() {
  to_lowercase(name);

  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "UPDATE restaurants SET name = " + placeholders.get();
  params.append(name);
  placeholders.next();
  query += ", reservation_service_code = " + placeholders.get();
  params.append(reservation_service_code_to_string(rsc));
  placeholders.next();
  query += ", resy_id = " + placeholders.get();
  params.append(resy_id);
  placeholders.next();
  query += " WHERE id = " + placeholders.get();
  params.append(id);

  tx.exec_params(query, params);

  tx.commit();
}

void Restaurant::save() {
  std::optional<Restaurant> restaurant = Restaurant::get(id);

  if (restaurant.has_value()) {
    update();
  } else {
    create();
  }
}

void Restaurant::refresh() {
  std::optional<Restaurant> restaurant = Restaurant::get(id);

  if (restaurant.has_value()) {
    *this = restaurant.value();
  } else {
    throw std::runtime_error("Restaurant does not exist");
  }
}

void Restaurant::remove() { remove(id); }

void Restaurant::remove(int id) {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query =
      "DELETE FROM restaurants WHERE id = " + placeholders.get();
  params.append(id);

  tx.exec_params(query, params);

  tx.commit();
}

void Restaurant::remove_all() {
  pqxx::work tx = get_work();

  std::string query = "DELETE FROM restaurants";
  tx.exec(query);

  tx.commit();
}
