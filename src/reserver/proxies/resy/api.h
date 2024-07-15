#pragma once

#include <boost/json.hpp>
#include <string>
#include <vector>

#include "src/reserver/proxies/resy/http_client.h"
#include "src/utils/geo/geo.h"
#include "src/utils/time/time.h"

/**
 * direct api to resy methods
 */
class ResyApi {
  ResyHttpClient http_client;

 public:
  ResyApi();

  class SearchInput {
   public:
    GeoCoordinates geo;
    std::string_view query;
  };

  class SearchOutputHit {
   public:
    std::string name;
    int id;
  };

  class SearchOutput {
   public:
    std::vector<SearchOutputHit> hits;
  };

  /**
   * endpoint to search for restaurants
   * api.resy.com/3/venuesearch/search
   */
  SearchOutput venue_search(SearchInput input);

  class FindInput {
   public:
    int venue_id;
    int party_size;
    Date day;
  };

  class FindOutputSlot {
   public:
    int id;
    std::string token;
    Time start_time;
    Time end_time;
    int min_size;
    int max_size;
  };

  class FindOutput {
   public:
    int id;
    std::vector<FindOutputSlot> slots;
  };

  /**
   * endpoint to find reservations
   * api.resy.com/4/find
   */
  FindOutput find(FindInput input);

  class DetailsInput {
   public:
    std::string_view config_id;
    Date day;
    int party_size;
  };

  class DetailsOutput {
   public:
    std::string book_token;
  };

  /**
   * endpoint to get details on a reservation
   * api.resy.com/3/details
   */
  DetailsOutput details(DetailsInput input);

  class BookInput {
   public:
    std::string_view book_token;
  };

  class BookOutput {
   public:
    std::string resy_token;
    int reservation_id;
  };

  /**
   * endpoint to book a reservation
   * api.resy.com/3/book
   */
  BookOutput book(BookInput);
};
