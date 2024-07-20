#pragma once

#include <string>

class GeoCoordinates {
public:
  double latitude;
  double longitude;
};

const GeoCoordinates new_york = {.latitude = 40.7128, .longitude = -74.0060};

class Locality {
public:
  // name of the location
  std::string name;

  GeoCoordinates coordinates;
};
