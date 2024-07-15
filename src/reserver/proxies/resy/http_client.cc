#include "src/reserver/proxies/resy/http_client.h"

#include "src/utils/config/config.h"
#include "src/utils/http_clients/http_json_client.h"

ResyHttpClient::ResyHttpClient() : HttpJsonClient(RESY_HOST) {}
