#pragma once

#include "src/utils/cli/cli_args.h"

/**
 * parses a command to interact with the http string client
 */
int http_string_handler(cli_args& args);

/**
 * parses a command to interact with the http json client
 */
int http_json_handler(cli_args& args);
