#include "src/reserver/proxies/resy/models/resy_restaurant.h"
#include "src/utils/db/db.h"
#include <optional>
#include <pqxx/pqxx>

void ResyRestaurant::create_table() {
  pqxx::work tx = get_work();

  std::string query = "CREATE TABLE IF NOT EXISTS resy_restaurants ("
                      " restaurant_id INT NOT NULL,"
                      " resy_id INT NOT NULL,"
                      " PRIMARY KEY (restaurant_id),"
                      " FOREIGN KEY (restaurant_id) REFERENCES restaurants(id)"
                      ")";

  tx.exec(query);

  tx.commit();
}

void ResyRestaurant::drop_table() {
  pqxx::work tx = get_work();

  std::string query = "DROP TABLE IF EXISTS resy_restaurants";
  tx.exec(query);

  tx.commit();
}

std::optional<ResyRestaurant> ResyRestaurant::get(int restaurant_id) {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "SELECT resy_id FROM resy_restaurants"
                      " WHERE restaurant_id = " +
                      placeholders.get();
  params.append(restaurant_id);

  auto r = tx.query01<int>(query, params);

  if (!r.has_value()) {
    return std::nullopt;
  }

  auto [resy_id] = r.value();

  return ResyRestaurant{.restaurant_id = restaurant_id, .resy_id = resy_id};
}

std::vector<ResyRestaurant> ResyRestaurant::get_all() {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "SELECT restaurant_id, resy_id "
                      "FROM resy_restaurants ORDER BY restaurant_id";

  auto r = tx.query<int, int>(query, params);

  std::vector<ResyRestaurant> resy_restaurants{};
  for (auto row : r) {
    auto [restaurant_id, resy_id] = row;
    resy_restaurants.push_back(ResyRestaurant{
        .restaurant_id = restaurant_id,
        .resy_id = resy_id,
    });
  }

  return resy_restaurants;
}

void ResyRestaurant::create() {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query =
      "INSERT INTO resy_restaurants (restaurant_id, resy_id) VALUES (" +
      placeholders.get() + ", ";
  params.append(restaurant_id);
  placeholders.next();
  query += placeholders.get() + ")";
  params.append(resy_id);

  tx.exec_params(query, params);

  tx.commit();
}

void ResyRestaurant::update() {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query =
      "UPDATE resy_restaurants SET resy_id = " + placeholders.get();
  params.append(resy_id);
  placeholders.next();
  query += " WHERE restaurant_id = " + placeholders.get();

  params.append(restaurant_id);

  tx.exec_params(query, params);

  tx.commit();
}

void ResyRestaurant::save() {
  std::optional<ResyRestaurant> rsu = ResyRestaurant::get(restaurant_id);

  if (rsu.has_value()) {
    update();
  } else {
    create();
  }
}

void ResyRestaurant::refresh() {
  std::optional<ResyRestaurant> rsu = ResyRestaurant::get(restaurant_id);

  if (rsu.has_value()) {
    *this = rsu.value();
  } else {
    throw std::runtime_error("ResyRestaurant not found");
  }
}

void ResyRestaurant::remove() { remove(restaurant_id); }

void ResyRestaurant::remove(int restaurant_id) {
  pqxx::work tx = get_work();

  pqxx::params params{};
  pqxx::placeholders placeholders{};
  std::string query = "DELETE FROM resy_restaurants WHERE "
                      "restaurant_id = " +
                      placeholders.get();

  params.append(restaurant_id);

  tx.exec_params(query, params);

  tx.commit();
}

void ResyRestaurant::remove_all() {
  pqxx::work tx = get_work();

  std::string query = "DELETE FROM resy_restaurants";
  tx.exec(query);

  tx.commit();
}
