#include "src/reserver/reserver_models/restaurant.h"
#include "src/utils/db/db.h"
#include "src/utils/string/string.h"
#include <optional>
#include <pqxx/pqxx>

void Restaurant::create_table() {
  pqxx::work tx = get_work();

  std::string query = "CREATE TABLE IF NOT EXISTS restaurants ("
                      " id SERIAL PRIMARY KEY,"
                      " name TEXT NOT NULL,"
                      " reservation_service_code TEXT"
                      ")";

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
  std::string query = "SELECT name, reservation_service_code FROM restaurants"
                      " WHERE id = " +
                      placeholders.get();
  params.append(id);

  auto r = tx.query01<std::string, std::string>(query, params);

  if (!r.has_value()) {
    return std::nullopt;
  }

  auto [name, rsc] = r.value();

  Restaurant restaurant;
  restaurant.id = id;
  restaurant.name = name;
  restaurant.rsc = string_to_reservation_service_code(rsc);

  return restaurant;
}

std::vector<Restaurant> Restaurant::get_by_name(std::string &name) {
  to_lowercase(name);

  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query =
      "SELECT id, name, reservation_service_code FROM restaurants"
      " WHERE name LIKE " +
      placeholders.get();
  params.append('%' + name + '%');
  query += " ORDER BY id";

  auto r = tx.query<int, std::string, std::string>(query, params);

  std::vector<Restaurant> restaurants;
  for (const auto &row : r) {
    auto [id, name, rsc] = row;
    Restaurant restaurant;
    restaurant.id = id;
    restaurant.name = name;
    restaurant.rsc = string_to_reservation_service_code(rsc);
    restaurants.push_back(restaurant);
  }

  return restaurants;
}

std::vector<Restaurant> Restaurant::get_all() {
  pqxx::work tx = get_work();

  std::string query =
      "SELECT id, name, reservation_service_code FROM restaurants ORDER BY id";
  auto r = tx.query<int, std::string, std::string>(query);

  std::vector<Restaurant> restaurants;
  for (const auto &row : r) {
    auto [id, name, rsc] = row;
    Restaurant restaurant;
    restaurant.id = id;
    restaurant.name = name;
    restaurant.rsc = string_to_reservation_service_code(rsc);
    restaurants.push_back(restaurant);
  }

  return restaurants;
}

void Restaurant::create() {
  to_lowercase(name);

  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query =
      "INSERT INTO restaurants (name, reservation_service_code) VALUES (" +
      placeholders.get() + ", ";
  params.append(name);
  placeholders.next();
  query += placeholders.get() + +") RETURNING id";
  params.append(reservation_service_code_to_string(rsc));

  auto r = tx.exec_params1(query, params);

  id = r[0].as<int>();

  tx.commit();
}

void Restaurant::update() {
  to_lowercase(name);

  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query =
      "UPDATE restaurants SET name = " + placeholders.get() + ", ";
  params.append(name);
  placeholders.next();
  query += " reservation_service_code = " + placeholders.get();
  params.append(reservation_service_code_to_string(rsc));
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
