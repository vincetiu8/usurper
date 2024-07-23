#include "src/concierge/concierge.h"
#include "src/concierge/models/booking_request.h"
#include "src/reserver/models/user.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>

namespace chrono = std::chrono;

void cron() {
  Concierge concierge = Concierge();

  std::vector<BookingRequest> requests = BookingRequest::get_all();

  tm tm = {};
  strptime("24-07-23 00:00:00", "%y-%m-%d %H:%M:%S", &tm);

  tm.tm_hour -= 1;
  time_t t = mktime(&tm);

  std::cout << "sleeping until " << std::ctime(&t) << std::endl;

  std::this_thread::sleep_until(chrono::system_clock::from_time_t(t) -
                                chrono::milliseconds(500));

  std::cout << "done sleeping, time to snipe!\n";

  const chrono::time_point<chrono::steady_clock> start =
      chrono::steady_clock::now();

  for (int i = 0; i < 10; ++i) {
    for (auto it = begin(requests); it != end(requests);) {
      BookingRequest request = *it;
      User user = User::get(request.user_id).value();

      bool success = concierge.attempt_booking(user, request);

      if (success) {
        requests.erase(it);
      } else {
        ++it;
      }
    }

    if (requests.size() == 0) {
      break;
    }

    std::this_thread::sleep_for(chrono::milliseconds(100));
  }

  const chrono::time_point<chrono::steady_clock> end =
      chrono::steady_clock::now();
  std::cout << "done sniping at "
            << (std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                      start) /
                1000)
                   .count()
            << "ms\n";
}