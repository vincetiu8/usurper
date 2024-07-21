#include <optional>

/**
 * represents a user timeslot booking
 */
class Booking {
public:
  int user_id;

  int timeslot_id;

  static void create_table();

  static void drop_table();

  static std::optional<Booking> get(int user_id, int timeslot_id);

  static std::vector<Booking> get_by_user_id(int user_id);

  static std::vector<Booking> get_all();

  void create();

  void remove();

  static void remove(int user_id, int timeslot_id);

  static void remove_by_user_id(int user_id);

  static void remove_all();
};