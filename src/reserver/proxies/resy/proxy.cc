#include "src/reserver/proxies/resy/proxy.h"
#include "src/reserver/proxies/resy/api.h"
#include "src/reserver/proxies/resy/models/resy_user.h"
#include <string_view>

ResyProxy::ResyProxy() : api{ResyApi()} {};

void ResyProxy::login(User user, std::string_view email,
                      std::string_view password) {
  ResyApi::LoginInput input{.email = email, .password = password};

  ResyApi::LoginOutput res = api.login(input);

  ResyUser resy_user{.user_id = user.id, .auth_token = res.auth_token};
  resy_user.save();
}

std::vector<Restaurant> ResyProxy::query_restaurants(std::string_view name) {

  ResyApi::SearchInput input{
      .geo = new_york,
      .query = name,
  };

  ResyApi::SearchOutput res = api.search(input);

  std::vector<Restaurant> restaurants;

  return {};
}
