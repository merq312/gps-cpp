#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
                          // in one cpp file
#include "gps.h"
#include <catch2/catch.hpp>

TEST_CASE("same coordinate returns zero", "[distanceBetween]") {
  REQUIRE(distanceBetween(0, 0, 0, 0) == 0);
}

TEST_CASE("accurate for very short (<100m) distances (+-10m)",
          "[distanceBetween]") {
  const double lat1 = 46.50242973131753;
  const double lon1 = -81.00582681659645;
  const double lat2 = 46.50241865394658;
  const double lon2 = -81.00517235762065;

  const double res = distanceBetween(lat1, lon1, lat2, lon2);
  const double actual = 0.048;

  REQUIRE(res > actual - 0.01);
  REQUIRE(res < actual + 0.01);
}

TEST_CASE("generally accurate at maximum distance", "[distanceBetween]") {
  const double lat1 = -90;
  const double lon1 = -180;
  const double lat2 = 90;
  const double lon2 = 180;

  const double res = distanceBetween(lat1, lon1, lat2, lon2);
  const double actual = 20002;

  REQUIRE(res > actual - 20);
  REQUIRE(res < actual + 20);
}
