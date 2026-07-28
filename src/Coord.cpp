#include "Coord.h"
#include <stdexcept>

Coord::Coord() : latitude{0.0}, longitude{0.0}, latitudeDir{'N'}, longitudeDir{'E'} {}

Coord::Coord(const Coord &other) : latitude{other.latitude}, longitude{other.longitude} {
    if (latitude < -90 || latitude > 90) {
        throw std::out_of_range("Coord::Coord(const Coord&) latitude");
    }
    if (longitude < -180 || longitude > 180) {
        throw std::out_of_range("Coord::Coord(const Coord&) longitude");
    }

    latitudeDir = (latitude >= 0) ? 'N' : 'S';
    longitudeDir = (longitude >= 0) ? 'E' : 'W';
}

Coord::Coord(double lat, double lon) : latitude{lat}, longitude{lon} {
    if (latitude < -90 || latitude > 90) {
        throw std::out_of_range("Coord::Coord(double, double) latitude");
    }
    if (longitude < -180 || longitude > 180) {
        throw std::out_of_range("Coord::Coord(double, double) longitude");
    }

    latitudeDir = (latitude >= 0) ? 'N' : 'S';
    longitudeDir = (longitude >= 0) ? 'E' : 'W';
}
