#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
                          // in one cpp file
#include "gps.h"
#include <catch2/catch.hpp>

TEST_CASE("throws for out of range latitude and/or longitude",
          "[distanceBetween]") {
  CHECK_THROWS_AS(distanceBetween(-91, 0, 0, 0), std::out_of_range);
  CHECK_THROWS_AS(distanceBetween(0, -200, 45, 90), std::out_of_range);
  CHECK_THROWS_AS(distanceBetween(70, 0, 100, 0), std::out_of_range);
  CHECK_THROWS_AS(distanceBetween(0, 200, 45, 300), std::out_of_range);
}

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

TEST_CASE("accurate for medium (~10km) distances (+-100m)",
          "[distanceBetween]") {
  const double lat1 = 46.50206126774254;
  const double lon1 = -81.00593296469764;
  const double lat2 = 46.52025216179274;
  const double lon2 = -80.9399890685659;

  const double res = distanceBetween(lat1, lon1, lat2, lon2);
  const double actual = 5.44;

  REQUIRE(res > actual - 0.1);
  REQUIRE(res < actual + 0.1);
}

TEST_CASE("accurate for long (~100km) distances (+-1km)", "[distanceBetween]") {
  const double lat1 = 46.50020381528717;
  const double lon1 = -80.9965127688377;
  const double lat2 = 43.64609063084991;
  const double lon2 = -79.38449120357627;

  const double res = distanceBetween(lat1, lon1, lat2, lon2);
  const double actual = 341;

  REQUIRE(res > actual - 1.0);
  REQUIRE(res < actual + 1.0);
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
