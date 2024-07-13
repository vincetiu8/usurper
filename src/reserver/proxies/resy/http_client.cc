#include "src/reserver/proxies/resy/http_client.h"

#include "src/utils/http_clients/http_json_client.h"

const std::string RESY_HOST = "api.resy.com";

ResyHttpClient::ResyHttpClient() : HttpJsonClient(RESY_HOST) {}
