#include "src/reserver/proxies/resy/http_client.h"

#include <string>

#include "src/utils/config/config.h"
#include "src/utils/http_clients/http_json_client.h"

ResyHttpClient::ResyHttpClient()
    : HttpJsonClient(
          RESY_HOST, "443",
          {{http::field::authorization,
            "ResyAPI api_key = \"" + std::string(RESY_API_KEY) + "\""}}) {}
