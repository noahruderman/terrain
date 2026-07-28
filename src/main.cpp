// Data from https://portal.opentopography.org/apidocs/#/Public/getUsgsDem
#include <cmath>
#include <format>
#include <iostream>
#include <numbers>
#include <string>
#include <vector>

#include "Coord.h"
#include "MapData.h"

std::ostream& operator<<(std::ostream &stream, const Coord &coord) {
    auto rep = std::format("({}°{}, {}°{})", std::abs(coord.latitude), coord.latitudeDir, std::abs(coord.longitude), coord.longitudeDir);
    return stream << rep;
}

Coord promptCoord() {
    std::cout << "Enter N/S-coord, then E/W-coord" << std::endl;
    double ns, ew;
    std::cin >> ns >> ew;
    return {ns, ew};
}

int main(int argc, char **argv) {
    std::string fileName = "larger-data.asc";
    if (argc > 1) {
        fileName = argv[1];
    }
    MapData map(fileName);

    std::cout << "Lower Left: " << map.llCoord << '\n';
    std::cout << "Upper Right: " << map.urCoord << "\n\n";

    auto initialCoord = promptCoord();
    std::pair<int,int> closest = map.closestDataPoint(initialCoord);
    std::cout << "closest coords: " << map.toCoordinate(closest) << "\nh=" << map.get(closest) << "\n\n";

    std::vector<double> dirs;
    for (double i = 0; i < 360; i += .5) {
        dirs.push_back(i * std::numbers::pi / 180);
    }

    std::vector<std::pair<double, double>> deltas(dirs.size());
    std::vector<std::vector<Coord>> positions(dirs.size());
    std::vector<std::vector<double>> heightLine(dirs.size());

    for (int i = 0; i < dirs.size(); i++) {
        deltas[i] = {cos(dirs[i]) * map.cellSize / 2, sin(dirs[i]) * map.cellSize / 2};
        Coord currCoord {initialCoord};

        closest = map.closestDataPoint(currCoord);
        heightLine[i] = {map.get(closest)};
        positions[i] = {currCoord};

        while (currCoord.longitude > map.x_ll && currCoord.longitude < map.x_ur && currCoord.latitude > map.y_ll && currCoord.latitude < map.y_ur) {
            closest = map.closestDataPoint(currCoord);
            double currHeight = map.get(closest);
            if (currHeight > heightLine[i].back()) {
                heightLine[i].push_back(currHeight);
                positions[i].push_back(currCoord);
            }

            currCoord.longitude += deltas[i].first;
            currCoord.latitude += deltas[i].second;
        }

        double largestAngle = 0;
        double la_dist = 0;
        int largestAngleIndex = 0;
        for (int j = 1; j < positions[i].size(); j++) {
            double deltaX = (positions[i][j].longitude - initialCoord.longitude) * MapData::DEG_TO_METERS;
            double deltaY = (positions[i][j].latitude - initialCoord.latitude) * MapData::DEG_TO_METERS;
            double deltaHeight = heightLine[i][j] - heightLine[i][0];
            double dist = sqrt(deltaX*deltaX + deltaY*deltaY);
            double angleDeg = atan(deltaHeight / dist) * 180 / std::numbers::pi;

            if (angleDeg > largestAngle) {
                largestAngle = angleDeg;
                largestAngleIndex = j;
                la_dist = dist;
            }
        }
        std::cout << "Direction: " << dirs[i]*180/std::numbers::pi << " deg" << '\n';
        // std::cout << heightLine[i].size() << " data points" << '\n';
        std::cout << "largest angle=" << largestAngle << ", h=" << heightLine[i][largestAngleIndex] << ", d=" << la_dist << " at ";
        std::cout << positions[i][largestAngleIndex] << "\n\n";
        // std::cout << dirs[i]/std::numbers::pi - 1 << ", " << largestAngle << ",\n";
    }

    return 0;
}
