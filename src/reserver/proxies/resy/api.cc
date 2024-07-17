#include "src/reserver/proxies/resy/api.h"

#include <boost/json.hpp>

#include "src/reserver/proxies/resy/http_client.h"

namespace json = boost::json;

ResyApi::ResyApi() : http_client{ResyHttpClient()} {};

ResyApi::LoginOutput ResyApi::login(LoginInput input) {
  std::string target = "/3/auth/password";
  std::string content = "email=";
  content += input.email;
  content += "&password=";
  content += input.password;

  json::value data = http_client.post_form_data(target, content);

  const json::object& data_obj = data.as_object();
  const json::string& token = data_obj.at("token").as_string();

  LoginOutput output{
      .token = token.c_str(),
  };

  return output;
};

ResyApi::SearchOutput ResyApi::venue_search(SearchInput input) {
  const std::string target = "/3/venuesearch/search";

  json::value content{
      {"geo",
       {
           {"latitude", input.geo.latitude},
           {"longitude", input.geo.longitude},
       }},
      {"query", input.query},
  };

  json::value data = http_client.post_json(target, content);

  const json::object& data_obj = data.as_object();
  const json::array& hits_arr = data_obj.at("search").at("hits").as_array();

  SearchOutput output{};
  output.hits.reserve(hits_arr.size());

  for (const json::value& hit : hits_arr) {
    SearchOutputHit hit_obj{
        .name = hit.at("name").as_string().c_str(),
        .id = static_cast<int>(hit.at("id").as_int64()),
    };
    output.hits.push_back(hit_obj);
  }

  return output;
};

ResyApi::FindOutput ResyApi::find(FindInput input) {
  std::string target = "/3/find";
  target += "?venue_id=";
  target += std::to_string(input.venue_id);
  target += "&party_size=";
  target += std::to_string(input.party_size);
  target += "&day=";
  target += input.day.to_yyyy_mm_dd_string();

  json::value data = http_client.get(target);

  const json::object& data_obj = data.as_object();
  const json::array& slots_arr =
      data_obj.at("results").at("venues").at(0).at("slots").as_array();

  FindOutput output{};
  output.slots.reserve(slots_arr.size());

  const std::string format = "%Y-%m-%d %H:%M:%S";

  for (const json::value& slot : slots_arr) {
    FindOutputSlot slot_obj{
        .id = static_cast<int>(slot.at("id").as_int64()),
        .token = slot.at("token").as_string().c_str(),
        .start_time = Time(slot.at("start_time").as_string().c_str(), format),
        .end_time = Time(slot.at("end_time").as_string().c_str(), format),
        .min_size = static_cast<int>(slot.at("min_size").as_int64()),
        .max_size = static_cast<int>(slot.at("max_size").as_int64()),
    };
    output.slots.push_back(slot_obj);
  }

  return output;
};

ResyApi::DetailsOutput ResyApi::details(DetailsInput input) {
  const std::string target = "/3/details";

  json::value content{
      {"commit", 1},
      {"config_id", input.config_id},
      {"party_size", input.party_size},
      {"day", input.day.to_yyyy_mm_dd_string()},
  };

  json::value data = http_client.post_json(target, content);

  const json::object& data_obj = data.as_object();
  const json::string& book_token =
      data_obj.at("book_token").at("value").as_string();

  DetailsOutput output{
      .book_token = book_token.c_str(),
  };

  return output;
};

ResyApi::BookOutput ResyApi::book(BookInput input) {
  std::string target = "/3/book";
  target += "?book_token=";
  target += input.book_token;

  json::value data = http_client.get(target);

  const json::object& data_obj = data.as_object();
  const json::string& resy_token = data_obj.at("resy_token").as_string();
  const int reservation_id =
      static_cast<int>(data_obj.at("reservation_id").as_int64());

  BookOutput output{
      .resy_token = resy_token.c_str(),
      .reservation_id = reservation_id,
  };

  return output;
};
