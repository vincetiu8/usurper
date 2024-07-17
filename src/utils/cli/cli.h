#pragma once

#include <string_view>
#include <vector>

/**
 * processes cli arguments into c++ 'safe' types
 */
void process_args(int argc, char *argv[], std::vector<std::string_view> &args);
