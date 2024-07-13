#pragma once

#include <string>
#include <utility>

class Locality {
  std::string name;
  // double latitude;
  // double longitude;
};

struct Location {
  double latitude;
  double longitude;
};

enum class DefinedLocation { new_york, philadelphia };

constexpr Location defined_location_to_location(DefinedLocation dl) {
  switch (dl) {
    case DefinedLocation::new_york:
      return {40.76464, -73.973};
    case DefinedLocation::philadelphia:
      return {40.020446, -75.14894};
  }
}
