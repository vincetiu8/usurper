#pragma once

#include "src/utils/time/time.h"
#include <optional>
#include <vector>

/**
 * represents a unique restaurant
 */
class Timeslot {
public:
  // primary key
  int id;

  int restaurant_id;

  Date date;

  Time start_time;

  Time end_time;

  int party_size;

  bool available;

  static void create_table();

  static void drop_table();

  static std::optional<Timeslot> get(int id);

  static std::vector<Timeslot> get_by_restaurant_id(int restaurant_id);

  static std::vector<Timeslot> get_all();

  void create();

  void update();

  void save();

  void refresh();

  void remove();

  static void remove(int id);

  static void remove_all();
};
