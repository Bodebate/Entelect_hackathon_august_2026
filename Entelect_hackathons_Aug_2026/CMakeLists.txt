cmake_minimum_required(version 3.15)
project(Entelect_hackathons_Aug_2026 CXX)

find_package(simdjson CONFIG REQUIRED)

add_executable(Entelect_hackathons_Aug_2026 main.cpp)
target_link_libraries(Entelect_hackathons_Aug_2026 PRIVATE simdjson::simdjson)