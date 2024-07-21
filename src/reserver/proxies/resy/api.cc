#include "src/reserver/proxies/resy/api.h"
#include "src/reserver/proxies/resy/http_client.h"
#include <boost/json.hpp>
#include <iostream>

namespace json = boost::json;

ResyApi::ResyApi() : http_client{ResyHttpClient()} {};

ResyApi::LoginOutput ResyApi::login(LoginInput input) {
  std::string target = "/3/auth/password";
  std::string content = "email=";
  content += input.email;
  content += "&password=";
  content += input.password;

  json::value data = http_client.post_form_data(target, content, {});

  const json::object &data_obj = data.as_object();
  const json::string &token = data_obj.at("token").as_string();

  LoginOutput output{
      .auth_token = token.c_str(),
  };

  return output;
};

ResyApi::SearchOutput ResyApi::search(SearchInput input) {
  const std::string target = "/3/venuesearch/search";

  json::value content{
      {"geo",
       {
           {"latitude", input.geo.latitude},
           {"longitude", input.geo.longitude},
       }},
      {"query", input.query},
  };

  json::value data = http_client.post_json(target, content, {});

  const json::object &data_obj = data.as_object();
  const json::array &hits_arr = data_obj.at("search").at("hits").as_array();

  SearchOutput output{};
  output.hits.reserve(hits_arr.size());

  for (const json::value &hit : hits_arr) {
    SearchOutputHit hit_obj{
        .name = hit.at("name").as_string().c_str(),
        .id = static_cast<int>(hit.at("id").at("resy").as_int64()),
    };
    output.hits.push_back(hit_obj);
  }

  return output;
};

ResyApi::FindOutput ResyApi::find(FindInput input) {
  std::string target = "/4/find?lat=0&long=0";
  target += "&day=";
  target += input.date.to_yyyy_mm_dd_string();
  target += "&party_size=";
  target += std::to_string(input.party_size);
  target += "&venue_id=";
  target += std::to_string(input.venue_id);

  json::value data = http_client.get(target, {});

  const json::object &data_obj = data.as_object();
  const json::array &slots_arr =
      data_obj.at("results").at("venues").at(0).at("slots").as_array();

  FindOutput output{};
  output.slots.reserve(slots_arr.size());

  const std::string format = "%Y-%m-%d %H:%M:%S";

  for (const json::value &slot : slots_arr) {
    FindOutputSlot slot_obj{
        .id = static_cast<int>(slot.at("config").at("id").as_int64()),
        .token = slot.at("config").at("token").as_string().c_str(),
        .start_time =
            Time(slot.at("date").at("start").as_string().c_str(), format),
        .end_time = Time(slot.at("date").at("end").as_string().c_str(), format),
        .min_size = static_cast<int>(slot.at("size").at("min").as_int64()),
        .max_size = static_cast<int>(slot.at("size").at("max").as_int64()),
    };
    output.slots.push_back(slot_obj);
  }

  return output;
};

ResyApi::DetailsOutput ResyApi::details(DetailsInput input) {
  const std::string target = "/3/details";

  json::value content{
      {"commit", 1},
      {"config_id", input.timeslot_token},
      {"day", input.day.to_yyyy_mm_dd_string()},
      {"party_size", input.party_size},
  };

  headers_t headers = {
      {"X-Resy-Auth-Token", std::string(input.auth_token)},
      {"X-Resy-Universal-Auth", std::string(input.auth_token)},
  };

  json::value data = http_client.post_json(target, content, headers);

  const json::object &data_obj = data.as_object();
  const json::string &book_token =
      data_obj.at("book_token").at("value").as_string();

  DetailsOutput output{
      .book_token = book_token.c_str(),
  };

  return output;
};

ResyApi::BookOutput ResyApi::book(BookInput input) {
  std::string target = "/3/book";
  std::string content = "book_token=";
  content += input.book_token;
  content += "&source_id=resy.com-venue-details&venue_marketing_opt_in=0";

  headers_t headers = {
      {"X-Resy-Auth-Token", std::string(input.auth_token)},
      {"X-Resy-Universal-Auth", std::string(input.auth_token)},
  };

  json::value data = http_client.post_form_data(target, content, headers);

  json::object &data_obj = data.as_object();
  const json::string &resy_token = data_obj.at("resy_token").as_string();

  target = "/3/user/reservations";
  target += "?resy_token=";
  target += resy_token;
  target += "&book_on_behalf_of=false";

  data = http_client.get(target, headers);
  data_obj = data.as_object();

  const json::object reservation_obj =
      data_obj.at("reservations").at(0).as_object();
  const json::string &booking_token =
      reservation_obj.at("resy_token").as_string();
  const int reservation_id =
      static_cast<int>(reservation_obj.at("reservation_id").as_int64());

  BookOutput output{
      .booking_token = booking_token.c_str(),
      .reservation_id = reservation_id,
  };

  return output;
};

void ResyApi::cancel(CancelInput input) {
  std::string target = "/3/cancel";
  std::string content = "resy_token=";
  content += input.booking_token;

  headers_t headers = {
      {"X-Resy-Auth-Token", std::string(input.auth_token)},
      {"X-Resy-Universal-Auth", std::string(input.auth_token)},
  };

  http_client.post_form_data(target, content, headers);
};
