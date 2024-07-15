#include "src/reserver/proxies/resy/api.h"

#include "src/reserver/proxies/resy/http_client.h"

ResyApi::ResyApi()
    : http_client{ResyHttpClient()} {

      };
