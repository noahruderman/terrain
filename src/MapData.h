#include "Coord.h"
#include <vector>

#pragma once

class MapData {
private:
    std::string unneeded;
public:
    static constexpr int DEG_TO_METERS = 3600 * 60;

    int cols, rows;
    double x_ll, y_ll; /* lower left corner */
    double x_ur, y_ur; /* upper right corner */
    Coord llCoord, urCoord;
    double cellSize;
    int NO_DATA_VALUE;

    std::vector<std::vector<double>> map;

    bool isMissingData;

    /* Separation between points in meters */
    double data_separation;

    MapData(const std::string &fileName);

    double get(const std::pair<int,int> &position) const {
        return get(position.first, position.second);
    }

    double get(int x, int y) const {
        return map.at(y).at(x);
    }

    Coord toCoordinate(const std::pair<int, int> &position) const {
        return toCoordinate(position.first, position.second);
    }

    Coord toCoordinate(int x, int y) const {
        return Coord(y_ur - cellSize * y, x_ll + cellSize * x);
    }

    std::pair<int, int> closestDataPoint(const Coord &coord) const;
};
