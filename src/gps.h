#pragma once

double distanceBetween(double lat1, double lon1, double lat2, double lon2);

class MockGPS {
public:
  double lat;
  double lon;
  double d_lat;
  double d_lon;

  MockGPS();
  void update_pos();
};
