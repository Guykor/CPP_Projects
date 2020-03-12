cmake_minimum_required(VERSION 3.15)
project(SpamDetector)

set(CMAKE_CXX_STANDARD 14)

add_executable(SpamDetector HashMap.hpp SpamDetector.cpp)

find_package(Boost COMPONENTS filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

target_link_libraries(SpamDetector ${Boost_LIBRARIES})