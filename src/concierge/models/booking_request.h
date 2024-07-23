#pragma once

#include "src/utils/time/time.h"

/**
 * represents a request to book a restaurant
 */
class BookingRequest {
public:
  int id;
  int user_id;
  int restaurant_id;
  Date date;
  Time earliest_time;
  Time latest_time;
  int party_size;

  static void create_table();

  static void drop_table();

  static std::optional<BookingRequest> get(int id);

  static std::vector<BookingRequest> get_by_user_id(int user_id);

  static std::vector<BookingRequest> get_all();

  void create();

  void update();

  void save();

  void refresh();

  void remove();

  static void remove(int id);

  static void remove_by_user_id(int user_id);

  static void remove_all();
};
