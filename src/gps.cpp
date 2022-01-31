#include "gps.h"
#include <cmath>
#include <random>

double degToRad(double degrees) {
  const double PI_DIVIDE_BY_180 = 0.0174532925199433;
  return degrees * PI_DIVIDE_BY_180;
}

double distanceBetween(double lat1, double lon1, double lat2, double lon2) {
  const auto earthRadius = 6371;

  const auto deltaLatDiv2 = degToRad(lat2 - lat1) / 2.0;
  const auto deltaLonDiv2 = degToRad(lon2 - lon1) / 2.0;

  lat1 = degToRad(lat1);
  lat2 = degToRad(lat2);

  const auto a = sin(deltaLatDiv2) * sin(deltaLatDiv2) +
                 sin(deltaLonDiv2) * sin(deltaLonDiv2) * cos(lat1) * cos(lat2);

  const auto c = 2 * atan2(sqrt(a), sqrt(1 - a));

  return earthRadius * c;
}

MockGPS::MockGPS() {
  std::random_device rd;
  std::mt19937_64 generator(rd());
  std::uniform_real_distribution<double> distribution(0.0, 1.0);
  this->lat = 45.0 + distribution(generator) / 100.0;
  this->lon = -81.0 + distribution(generator) / 100.0;
  this->d_lat = (distribution(generator) - 0.5) / 1000.0;
  this->d_lon = (distribution(generator) - 0.5) / 1000.0;
}

void MockGPS::update_pos() {
  this->lat += this->d_lat;
  this->lon += this->d_lon;
}
