#include "src/reserver/proxies/resy/http_client.h"
#include "src/utils/config/config.h"
#include "src/utils/http_clients/http_json_client.h"
#include <string>

ResyHttpClient::ResyHttpClient()
    : HttpJsonClient(
          RESY_HOST, "443",
          {{"Host", "api.resy.com"},
           {"User-Agent",
            "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 "
            "(KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.3"},
           {"Authorization",
            "ResyAPI api_key=\"" + std::string(RESY_API_KEY) + "\""},
           {"Origin", "https://resy.com"},
           {"Referer", "https://resy.com"},
           {"X-Origin", "https://resy.com"}}) {}
