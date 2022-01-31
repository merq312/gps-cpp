#pragma once

#include <random>

double distanceBetween(double lat1, double lon1, double lat2, double lon2);

class MockGPS {
public:
    double lat;
    double lon;
    double d_lat;
    double d_lon;

    MockGPS() {
        std::random_device rd;
        std::mt19937_64 generator(rd());
        std::uniform_real_distribution<double> distribution(0.0, 1.0);
        this->lat = distribution(generator) / 100.0;
        this->lon = distribution(generator) / 100.0;
        this->d_lat = (distribution(generator) - 0.5) / 1000.0;
        this->d_lon = (distribution(generator) - 0.5) / 1000.0;
    }

    void update_pos() {
        this->lat += this->d_lat;
        this->lon += this->d_lon;
    }
};
