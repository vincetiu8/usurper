#pragma once

#include <string>

class GeoCoordinates {
 public:
  double latitude;
  double longitude;
};

class Locality {
 public:
  // name of the location
  std::string name;

  GeoCoordinates coordinates;
};
