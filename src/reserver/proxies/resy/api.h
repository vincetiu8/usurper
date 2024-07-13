#pragma once

#include <string>
#include <vector>

#include "src/utils/geo/geo.h"

class ResyApiVenueSchema {
  std::string name;
  std::string url_slug;
};

class ResyApiReservationSchema {};

/**
 * direct api to resy methods
 */
class ResyApi {
  /**
   * endpoint to search for restaurants
   * api.resy.com/3/venuesearch/search
   */
  std::vector<ResyApiVenueSchema> venue_search(std::string query, Location loc);

  /**
   * endpoint to find reservations
   * api.resy.com/4/find
   */
  std::vector<ResyApiReservationSchema> find(int venue_id);

  /**
   * endpoint to get details on a reservation
   * api.resy.com/3/details
   */
  std::string details(std::string config_id);

  /**
   * endpoint to book a reservation
   * api.resy.com/3/book
   */
  int book(std::string book_token, int payment_method_id);
};
