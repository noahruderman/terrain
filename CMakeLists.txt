cmake_minimum_required(VERSION 3.10)
project(Terrain)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(terrain src/main.cpp src/Coord.cpp src/MapData.cpp)
