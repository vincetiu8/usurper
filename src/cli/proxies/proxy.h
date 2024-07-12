#pragma once

#include <string_view>
#include <vector>

#include "src/utils/cli/cli_args.h"

/**
 * parses a command to interact with one of the proxy APIs
 */
int proxy_handler(cli_args& args);
