#pragma once
#include <stdexcept>
#include <string>

/**
 * unique code for each reservation service in system
 */
enum ReservationServiceCode {
  resy,
};

inline std::string
reservation_service_code_to_string(ReservationServiceCode code) {
  switch (code) {
  case ReservationServiceCode::resy:
    return "resy";
  default:
    throw std::invalid_argument("invalid reservation service code");
  }
}

inline ReservationServiceCode
string_to_reservation_service_code(std::string code) {
  if (code == "resy") {
    return ReservationServiceCode::resy;
  }

  throw std::invalid_argument("invalid reservation service code");
}
