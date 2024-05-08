cmake_minimum_required(VERSION 3.5)

project(proiect2sd)

set(CMAKE_CXX_STANDARD 11)

file(GLOB SOURCES "src/*.cpp" "main.cpp")

add_executable(proiect2sd ${SOURCES})

target_include_directories(proiect2sd PUBLIC include)

target_link_libraries(proiect2sd)

install(TARGETS proiect2sd DESTINATION bin)