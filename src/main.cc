#include "booker/booker.h"
#include "concierge/concierge.h"
#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include <boost/beast.hpp>

int main() {
    std::cout << booker() << concierge() << "\n";
    return 0;
}
