#pragma once

#include "src/reserver/proxies/resy/http_client.h"
#include "src/utils/geo/geo.h"
#include "src/utils/time/time.h"
#include <boost/json.hpp>
#include <string>
#include <vector>

/**
 * direct api to resy methods
 */
class ResyApi {
  ResyHttpClient http_client;

public:
  ResyApi();

  class LoginInput {
  public:
    std::string_view email;
    std::string_view password;
  };

  class LoginOutput {
  public:
    std::string auth_token;
  };

  /**
   * endpoint to login
   */
  LoginOutput login(LoginInput input);

  class SearchInput {
  public:
    std::string_view auth_token;
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
   */
  SearchOutput search(SearchInput input);

  class FindInput {
  public:
    std::string_view auth_token;
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
   */
  FindOutput find(FindInput input);

  class DetailsInput {
  public:
    std::string_view auth_token;
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
   */
  DetailsOutput details(DetailsInput input);

  class BookInput {
  public:
    std::string_view auth_token;
    std::string_view book_token;
  };

  class BookOutput {
  public:
    std::string resy_token;
    int reservation_id;
  };

  /**
   * endpoint to book a reservation
   */
  BookOutput book(BookInput);
};
