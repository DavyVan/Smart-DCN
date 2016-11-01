cmake_minimum_required(VERSION 2.8.4)
project(edmonds)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11 -DNDEBUG")

set(SOURCE_FILES main.cpp EdmondsMatching.cpp Vertex.cpp Blossom.cpp Edge.cpp)
add_executable(edmonds ${SOURCE_FILES})

set(SOURCE_FILES test.cpp EdmondsMatching.cpp Vertex.cpp Blossom.cpp Edge.cpp)
add_executable(edmonds_test ${SOURCE_FILES})
