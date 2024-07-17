#pragma once

#include "src/utils/http_clients/http_json_client.h"

/**
 * wrapper around HttpClient for the resy endpoint
 */
class ResyHttpClient : public HttpJsonClient {
 public:
  ResyHttpClient();
};
